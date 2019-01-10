#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
// #include <string>

namespace Coup_de_Grace{
    using namespace eosio;
    using std::string;

    class Surprise : public contract { // manage actions about Surprise
        using contract::contract;   //in order to use its constructor 
        public:
            Surprise(account_name self):contract(self) {}

            [[eosio::action]]
            void showinfo(){
                print("Surprise info");
            }

    };

    EOSIO_ABI(Surprise, (showinfo))
}
