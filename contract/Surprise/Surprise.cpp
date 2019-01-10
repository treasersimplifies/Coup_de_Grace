#include "Surprise.hpp"


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
        print(" You have successfully create for an surpriseprjs in Coup_de_Grace!");
    }

    [[eosio::action]]
    void Surprise::checkbyid(const account_name author, uint64_t project_id){
        // print("Inside checkbyid...");
        surpriseprjIndex surpriseprjs(_self, _self);
        auto iterator = surpriseprjs.find(project_id);
        eosio_assert(iterator != surpriseprjs.end(), "Project not found.");

        auto theprj = surpriseprjs.get(project_id);
        print("|-| Id: ", theprj.id);
        print(" | Name: ", theprj.name.c_str());
        print(" | Items : ");
        if (theprj.items.size() > 0) {
            for (uint32_t i = 0; i < theprj.items.size(); i++) {
                // ATN： theprj.items.at(0).id = 1 ！！！！
                print("id:",theprj.items.at(i).id, " name:");
                print(theprj.items.at(i).name.c_str(), " winumber:");
                print(theprj.items.at(i).winumber, " winners:");
                if (theprj.items.at(i).winners.size() > 0) {
                    for (uint32_t j = 0; j < theprj.items.at(i).winners.size(); j++) {
                        print(theprj.items.at(i).winners.at(j)," ");
                    }
                }else {
                    print("not generated yet ");
                }
            }
        } else {
            print("Undefined ");
        }
    }

    [[eosio::action]]
    void Surprise::checkn(const account_name author,int number){
        surpriseprjIndex surpriseprjs(_self, _self);
        for(int project_id = 1; project_id <= number; project_id++){
            auto iterator = surpriseprjs.find(project_id);
            eosio_assert(iterator != surpriseprjs.end(), "Project not found.");
            auto theprj = surpriseprjs.get(project_id);
            print("|-| Id: ", theprj.id);
            print(" | Name: ", theprj.name.c_str());
            if (theprj.items.size() > 0) {
                print(" |Items : ");
                for (uint32_t i = 0; i < theprj.items.size(); i++) {
                    print("id:",theprj.items.at(i).id, " name:");
                    print(theprj.items.at(i).name.c_str(), " winumber:");
                    print(theprj.items.at(i).winumber, " winnners:");
                    if (theprj.items.at(i).winners.size() > 0) {
                        for (uint32_t j = 0; j < theprj.items.at(i).winners.size(); j++) {
                            print(theprj.items.at(i).winners.at(j)," ");
                        }
                    }else {
                        print("not generated yet ");
                    }
                }
            } else {
                print(" |Items : Undefined ");
            }
        }
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

    }

    [[eosio::action]]
    void Surprise::addcad(const account_name author, uint64_t project_id, uint64_t item_id, string& cadname){
        surpriseprjIndex surpriseprjs(_self, _self);
        auto iterator = surpriseprjs.find(project_id);
        eosio_assert(iterator != surpriseprjs.end(), "Project not found.");

        surpriseprjs.modify(iterator, author, [&](auto& surpriseprj) {
            surpriseprj.items[item_id-1].cadidates.push_back(cadname);
        });
    }
    
    [[eosio::action]]
    void Surprise::activate(const account_name author, uint64_t project_id, uint64_t item_id){
        surpriseprjIndex surpriseprjs(_self, _self);
        auto iterator = surpriseprjs.find(project_id);
        eosio_assert(iterator != surpriseprjs.end(), "Project not found.");

        auto theprj = surpriseprjs.get(project_id);
        auto theitem = theprj.items[item_id-1];

        uint32_t cadnumber = theitem.cadidates.size();
        uint32_t winumber = theitem.winumber;
        print(theitem.winumber," out of ",cadnumber," will win.They are ");
        // surpriseprjs.modify(iterator, author, [&](auto& surpriseprj) {
        //     surpriseprj.items.find(item_id).push_back(winner);
        // });

        int lucky[5]={0,0,0,0,0};
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
        print(". ");

        for(int i=0; i<winumber; i++){
            string winner = theitem.cadidates[lucky[i]-1];
            surpriseprjs.modify(iterator, author, [&](auto& surpriseprj) {
                surpriseprj.items[item_id-1].winners.push_back(winner);
            });
        }

    }


}
