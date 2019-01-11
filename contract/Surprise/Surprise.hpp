#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <string>
#include <eosiolib/crypto.h>
#include <eosiolib/transaction.hpp>

namespace Coup_de_Grace{
    using namespace eosio;
    using std::string;

    class Surprise : public contract { // manage actions about Surprise
        using contract::contract;   //in order to use its constructor 

        public:
            Surprise(account_name self):contract(self){}

            [[eosio::action]]
            void showinfo(){
                print("Surprise!!!! It is a random draw/lottery dapp based on eos blockchain.");
                surpriseprjIndex surpriseprjs(_self, _self);
                int project_id = 1;
                for(project_id = 1; ; project_id++){
                    auto iterator = surpriseprjs.find(project_id);
                    if(iterator == surpriseprjs.end()){
                        break;
                    }
                }
                print("Numbers of Projects:",project_id-1);
            }
            struct surpriseitem{
                uint64_t id;
                string name;
                uint32_t winumber; 
                uint32_t maxnumber;
                vector<string> cadidates;//phone number
                vector<string> winners;//phone number

                uint64_t primary_key() const { return id; }
                // EOSLIB_SERIALIZE(surpriseitem, (id)(name)(number))
            };
            struct surpriseprj{
                uint64_t id;
                string name;
                vector<surpriseitem> items;//
            
                uint64_t primary_key() const { return id; }
                // EOSLIB_SERIALIZE(surpriseprj, (id)(name))
            };
            [[eosio::action]]
            void create(const account_name author, uint64_t project_id, string& project_name);
            [[eosio::action]]
            void checkbyid(const account_name author, uint64_t project_id);
            [[eosio::action]]
            void checkn(const account_name author,int number);
            [[eosio::action]]
            void additem(const account_name author, uint64_t project_id, uint64_t item_id, string& item_name, uint32_t winumber, uint32_t maxnumber);
            // [[eosio::action]]
            // void modifitem(const account_name author, uint64_t project_id, uint64_t item_id, string& item_name, uint32_t winumber, uint32_t maxnumber);
            [[eosio::action]]
            void addcad(const account_name author, uint64_t project_id, uint64_t item_id, string& cadname);
            [[eosio::action]]
            void activate(const account_name author, uint64_t project_id, uint64_t item_id);

            typedef multi_index<N(surpriseprj), surpriseprj> surpriseprjIndex;
    };

    EOSIO_ABI(Surprise, (showinfo)(create)(checkbyid)(checkn)(additem)(addcad)(activate))//(modifitem)
}
