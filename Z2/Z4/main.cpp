//TP 2023/2024: Zadaća 2, Zadatak 4
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
template <typename Tip>
bool Kriterij(const std::vector<Tip> &a, const std::vector<Tip> &b)
{
    Tip max_a=*std::max_element(a.begin(),a.end());
    Tip max_b=*std::max_element(b.begin(),b.end());

    if(max_a !=max_b)
    return max_a>max_b;
    else return a>b;
}

template <typename Tip>
void SortirajPoDobrotiRedova(std::vector<std::vector<Tip>> &matrica){
    std::sort(matrica.begin(),matrica.end(),Kriterij<Tip>);
}


int main ()
{
    std::vector<std::vector<int>> matrica;
    std::string unos;
    char k;
    std::cout<<"Unesite elemente (* za kraj reda, * na pocetku reda za kraj unosa):\n";
    while(true){
        std::vector<int> red;
        int broj;
        while(std::cin>>broj)
        {
            red.push_back(broj);
        }
        if(red.empty())break;
        matrica.push_back(red);
        std::cin.clear();
        std::cin>>k;
    }
    std::cout<<std::endl;
    SortirajPoDobrotiRedova(matrica);
    std::cin.clear();
    std::cin.ignore(10000,'\n');
    std::cout<<"Matrica nakon sortiranja:\n";
    for(auto&r :matrica){
        for(auto &n : r){
            std::cout<<n<<" ";
        }
        std::cout<<std::endl;
    }
    std::cout<<std::endl;
    std::cout<<"Unesite elemente sekvence koja se trazi (* za kraj reda): \n";
    std::vector<int> sekvenca;
    int broj;
    while(std::cin>>broj){
        sekvenca.push_back(broj);
    }
    auto it=std::lower_bound(matrica.begin(), matrica.end(), sekvenca, Kriterij<int>);
    if(it!=matrica.end() && *it==sekvenca){
        std::cout<<"Trazena sekvenca se nalazi u "<<std::distance(matrica.begin(), it)+1<<". redu (nakon sortiranja)\n";
    }else {
        std::cout<<"Trazena sekvenca se ne nalazi u matrici\n";
    }




	return 0;
}
