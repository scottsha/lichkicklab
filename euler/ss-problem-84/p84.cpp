#include <algorithm>
#include <iostream>
#include <ostream>
#include <vector>

#include <Eigen/Dense>

using Eigen::MatrixXd;

/*
 Compute the probability distribution of the squares of a Monopoly board as a markov chain stable distribution.
 */

// C++ template to print vector container elements
template <typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
    os << std::endl;
    for (int i = 0; i < v.size(); ++i) {
        os << v[i];
        if (i != v.size() - 1)
            os << ", ";
    }
    os << std::endl;
    return os;
}

std::vector<int> polynomial_multiply(std::vector<int> p, std::vector<int> q){
    // Multiply integer polynomials represented as coefficient lists
    std::vector<int> r(p.size() + q.size()-1);
    // SS: Use FFT would be faster I guess, but I'm just directly convolving
    for (auto foo=0; foo<p.size(); ++foo){
        for (auto bar=0; bar<=q.size(); ++bar){
            r[foo+bar] += p[foo] * q[bar];
        }
    }
    return r;
}


Eigen::VectorXd roll_distribution(const int rolls, const int die = 6){
    // Get the probability distribution of repeated roll of a fixed-size die
    std::vector<int> gen_poly_rol(die+1, 1);
    gen_poly_rol[0]=0;
    std::vector<int> gen_poly_rolls = {1};
    for (auto foo = 0; foo < rolls; ++foo){
        gen_poly_rolls = polynomial_multiply(gen_poly_rolls, gen_poly_rol);
    }

    Eigen::VectorXd dist(gen_poly_rolls.size());
    double denom =  pow(die, rolls);
    for (auto foo =0;  foo< gen_poly_rolls.size(); ++foo) dist(foo) = gen_poly_rolls[foo]/denom;
    return dist;
}

Eigen::VectorXd non_double_roll_distribution(const int die = 6){
    // Distribution without double rolls, as per Monopoly rules
    std::vector<int> roll_dist(2*die+1, 0);
    for (auto foo=1; foo<= die; ++foo){
        for (auto bar=1; bar<foo; ++bar){
            roll_dist[foo+bar] += 2;
        }
    }
    Eigen::VectorXd dist(roll_dist.size());
    double denom =  pow(die, 2) - die;
    for (auto foo =0;  foo< roll_dist.size(); ++foo) dist(foo) = roll_dist[foo]/denom;
    return dist;
}


Eigen::VectorXd monopoly_roll_distribution(const int die){
    // Probability distribution of number of squares to advance from a die roll
    Eigen::VectorXd roll_dist = roll_distribution(2, die);
    Eigen::VectorXd nd_roll_dist = non_double_roll_distribution(die);
    Eigen::VectorXd dist = (1 - 1.0 / pow(die,2)) * roll_dist;
    dist += (1 - 1.0 / die) / pow(die,2) * nd_roll_dist;
    dist(0) += 1.0/pow(die,3);
    return dist;
}

// STATES OF THE MARKOV CHAIN
/*
 GO
 A1
 2: CC1 community chest
 A2
 T1
 R1
 B1
 CH1
 B2
 B3
 10: JAIL
 C1
 U1
 C2
 C3
 R2
 D1
 17: CC2
 D2
 D3
 FP
 E1
 CH2
 E2
 E3
 R3
 F1
 F2
 U2
 F3
 G2J
 G1
 G2
 33: CC3
 G3
 R4
 CH3
 H1
 T2
 H2
*/

//Special states
const std::vector<int> community_chest_locs = {2, 17, 33};

const std::vector<int> chance_locs = {7, 22, 36};

const int jail = 10;

const int go_directly_to_jail = 30;

const int sq_c1 = 11;

const int sq_e3 = 24;

const int sq_h2 = 39;

const int sq_r1 = 5;

int get_next_railway(const int at){
    if (at < 5) return 5;
    if (at < 15) return 15;
    if (at < 25) return 25;
    if (at < 35) return 35;
    return 5;
}

int get_next_utility(const int at){
    if (at < 12) return 12;
    if (at < 28) return 28;
    return 12;
}

Eigen::VectorXd community_chest_distribution(const int at){
    Eigen::VectorXd dist(40);
    dist.fill(0.0);
    dist(at) = 14.0/16;
    dist(0) = 1.0/16;
    dist(jail) = 1.0/16;
    return dist;
}

Eigen::VectorXd chance_distribution(const int at){
    // Distribution of chance cards
    Eigen::VectorXd dist(40);
    dist.fill(0.0);
    dist(at) = 6.0/16;
    dist(0) += 1.0/16;
    dist(jail) += 1.0/16;
    // go to C1
    dist(sq_c1) += 1.0/16;
    dist(sq_e3) += 1.0/16;
    dist(sq_h2) += 1.0/16;
    dist(sq_r1) += 1.0/16;
    dist(get_next_railway(at)) += 2.0/16;
    dist(get_next_utility(at)) += 1.0/16;
    int back3 = (at + 40 - 3)%40;
    dist(back3) += 1.0/16;
    return dist;
}

Eigen::VectorXd sq_distribution(const int at, const int die=6){
    Eigen::VectorXd dist(40);
    dist.fill(0);
    auto rd = monopoly_roll_distribution(die);
    dist(jail) += rd(0);
    for (auto foo=1; foo<rd.size(); foo++){
        int target = (at + foo) % 40;
        dist(target) += rd(foo);
    }
    // correct for go to jail
    dist(jail) += dist(go_directly_to_jail);
    dist(go_directly_to_jail) = 0.0;
    // correct for chance
    for (const auto c_sq : chance_locs){
        double prob = dist(c_sq);
        if (prob>0) {
            dist(c_sq) = 0;
            auto cdat = chance_distribution(c_sq);
            dist +=  prob * cdat;
        }
    }
    // correct for community chest
    for (const auto c_sq : community_chest_locs){
        double prob = dist(c_sq);
        if (prob > 0) {
            dist(c_sq) = 0;
            auto cdat = community_chest_distribution(c_sq);
            dist += prob * cdat;
        }
    }
//    //
    return dist;
}

Eigen::MatrixXd monopoly_markov(const int die){
    Eigen::MatrixXd mar(40,40);
    mar.fill(40);
    for (auto foo=0; foo<40; foo++){
        mar.col(foo) = sq_distribution(foo,die);
    }
    return mar;
}

Eigen::VectorXd stable_monopoly_distribution(const int die){
    // Perron-Frobenius should guarantee the state converges under markov evolution
    // we could extract the eigenstate, or just iterate.
    Eigen::VectorXd dist(40);
    dist.fill(1.0/40);
    //
    auto mm = monopoly_markov(die);
    //
    double distance_traveled = 1.0;
    while(distance_traveled > 1e-8){
        auto n_dist = mm * dist;
        distance_traveled = (dist - n_dist).norm();
        dist = n_dist;
    }
    return dist;
}


void print_rank_order_list(std::vector<double> x){
    // Print the monopoly squares ordered by likelihood
    std::vector<std::pair<int,double>> pairs(x.size());
    for (auto foo=0; foo< x.size(); foo++){
        pairs[foo] = {foo,x[foo]};
    };
    std::sort(pairs.begin(),
              pairs.end(),
              [](std::pair<int,double> a, std::pair<int,double> b){
                  return a.second > b.second;
              }
    );
    std::cout << "The ranking is: " << std::endl;
    for (const auto p : pairs){
        std::cout << p.first << " " << p.second << std::endl;
    }
}

int main()
{
    auto dist = stable_monopoly_distribution(4);
    std::vector<double> v2;
    v2.resize(dist.size());
    Eigen::VectorXd::Map(&v2[0], dist.size()) = dist;
    print_rank_order_list(v2);
    return 0;
}
