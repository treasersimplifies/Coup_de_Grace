#include "Surprise.hpp"

const int MAX_CAD = 25;

namespace Coup_de_Grace {
    
    [[eosio::action]]
    void Surprise::create(const account_name author, uint64_t project_id, string& project_name){
        // print("Inside create...");
        // require_auth(account);
        surpriseprjIndex surpriseprjs(_self, _self);
        auto iterator = surpriseprjs.find(project_id);
        eosio_assert(iterator == surpriseprjs.end(), " SurpriseProject already exists");

        surpriseprjs.emplace(author, [&](auto& surpriseprj) {
            surpriseprj.id = project_id;
            surpriseprj.name = project_name;
        });
        print("Successfully create a surpriseprjs named <strong>",project_name,"</strong> in Coup_de_Grace!");
    }

    [[eosio::action]]
    void Surprise::checkbyid(const account_name author, uint64_t project_id){
        // print("Inside checkbyid...");
        surpriseprjIndex surpriseprjs(_self, _self);
        auto iterator = surpriseprjs.find(project_id);
        eosio_assert(iterator != surpriseprjs.end(), "Project not found.");

        auto theprj = surpriseprjs.get(project_id);
        print("||| Id: ", theprj.id);
        print(" ||- Name: ", theprj.name.c_str());
        print(" ||- Items: ");
        if (theprj.items.size() > 0) {
            for (uint32_t i = 0; i < theprj.items.size(); i++) {
                // ATN： theprj.items.at(0).id = 1 ！！！！
                print("||id:",theprj.items.at(i).id, "  context:");
                print(theprj.items.at(i).name.c_str(), "  winners' number:");
                print(theprj.items.at(i).winumber,"  cadidates' number:");
                print(theprj.items.at(i).cadidates.size(), "  |-winners:");
                if (theprj.items.at(i).winners.size() > 0) {
                    for (uint32_t j = 0; j < theprj.items.at(i).winners.size(); j++) {
                        print(theprj.items.at(i).winners.at(j)," ");
                    }
                }else {
                    print("not generated yet ");
                }
            }
        } else {
            print(" Undefined ");
        }
        print("\n");
    }

    [[eosio::action]]
    void Surprise::checkn(const account_name author,int number){
        surpriseprjIndex surpriseprjs(_self, _self);
        for(int project_id = 1; project_id <= number; project_id++){
            auto iterator = surpriseprjs.find(project_id);
            eosio_assert(iterator != surpriseprjs.end(), "Project not found.");
            auto theprj = surpriseprjs.get(project_id);
            print("||| Id: ", theprj.id);
            print(" ||- Name: ", theprj.name.c_str());
            print(" ||- Items: ");
            if (theprj.items.size() > 0) {
                for (uint32_t i = 0; i < theprj.items.size(); i++) {
                    print("||id:",theprj.items.at(i).id, "  context:");
                    print(theprj.items.at(i).name.c_str(), "  winners' number:");
                    print(theprj.items.at(i).winumber,"  cadidates' number:");
                    print(theprj.items.at(i).cadidates.size(), "  |-winners:");
                    if (theprj.items.at(i).winners.size() > 0) {
                        for (uint32_t j = 0; j < theprj.items.at(i).winners.size(); j++) {
                            print(theprj.items.at(i).winners.at(j)," ");
                        }
                    }else {
                        print("not generated yet ");
                    }
                }
            } else {
                print(" Undefined ");
            }
        }
        print("\n");
    }

    [[eosio::action]]
    void Surprise::additem(const account_name author, uint64_t project_id, uint64_t item_id, string& item_name, uint32_t winumber, uint32_t maxnumber){
        surpriseprjIndex surpriseprjs(_self, _self);
        auto iterator = surpriseprjs.find(project_id);
        eosio_assert(iterator != surpriseprjs.end(), "Project not found.");

        surpriseprjs.modify(iterator, author, [&](auto& surpriseprj) {
            surpriseprj.items.push_back(surpriseitem{
                item_id,
                item_name,
                winumber,
                maxnumber
            });
        });
        print("Successfully add an item named <strong>",item_name,"</strong>!");
    }

    [[eosio::action]]
    void Surprise::addcad(const account_name author, uint64_t project_id, uint64_t item_id, string& cadname){
        surpriseprjIndex surpriseprjs(_self, _self);
        auto iterator = surpriseprjs.find(project_id);
        eosio_assert(iterator != surpriseprjs.end(), "Project not found.");

        surpriseprjs.modify(iterator, author, [&](auto& surpriseprj) {
            surpriseprj.items[item_id-1].cadidates.push_back(cadname);
        });
        print("Successfully add a cadidate named <strong>",cadname,"</strong>!");
    }
    
    [[eosio::action]]
    void Surprise::activate(const account_name author, uint64_t project_id, uint64_t item_id){
        surpriseprjIndex surpriseprjs(_self, _self);
        auto iterator = surpriseprjs.find(project_id);
        eosio_assert(iterator != surpriseprjs.end(), "Project not found.");

        auto theprj = surpriseprjs.get(project_id);
        auto theitem = theprj.items[item_id-1];
        eosio_assert(theitem.winners.size()==0, "This item already activated!!! No Twicy!!!");

        uint32_t cadnumber = theitem.cadidates.size();
        uint32_t winumber = theitem.winumber;
        print(theitem.winumber," out of ",cadnumber," will win.They are ");

        int lucky[MAX_CAD];//
        if(winumber<=cadnumber){
            for(int i=0; i<cadnumber; ){
                checksum256 result;
                auto mixedBlock = tapos_block_prefix() * tapos_block_num();
                const char *mixedChar = reinterpret_cast<const char *>(&mixedBlock);
                sha256( (char *)mixedChar, sizeof(mixedChar), &result);
                const char *p64 = reinterpret_cast<const char *>(&result);
                auto r = (abs((int64_t)p64[i]) % (cadnumber + 1 - 1)) + 1;  //1 to cadnumber
                int need_to_repeat=0;
                for(int j=0;j<5;j++){
                    if(r==lucky[j]){
                        need_to_repeat=1;
                        break;
                    }
                }
                if(need_to_repeat==0){
                    print(" ", r);
                    lucky[i++]=r;
                }
                if(i==winumber)break;
            }
        }else{
            for(int i=0;i<cadnumber;i++){
                lucky[i]=i+1;
                print(" ", i+1);
            }
        }
        print(". ");

        for(int i=0; i<winumber; i++){
            string winner = theitem.cadidates[lucky[i]-1];
            surpriseprjs.modify(iterator, author, [&](auto& surpriseprj) {
                surpriseprj.items[item_id-1].winners.push_back(winner);
            });
        }

    }


}
