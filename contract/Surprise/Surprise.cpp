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
        if (theprj.items.size() > 0) {
            print(" |Items : ");
            for (uint32_t i = 0; i < theprj.items.size(); i++) {
                print("id.",theprj.items.at(i).id, " name.");
                print(theprj.items.at(i).name.c_str(), " winumber.");
                print(theprj.items.at(i).winumber, " ");
            }
        } else {
            print(" |Items : Undefined ");
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
                    print("id.",theprj.items.at(i).id, " name.");
                    print(theprj.items.at(i).name.c_str(), " winumber.");
                    print(theprj.items.at(i).winumber, " ");
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
            surpriseprj.items.find(item_id).push_back(cadname);
        });
    }

    [[eosio::action]]
    void Surprise::activate(const account_name author, uint64_t project_id, uint64_t item_id){
        surpriseprjIndex surpriseprjs(_self, _self);
        auto iterator = surpriseprjs.find(project_id);
        eosio_assert(iterator != surpriseprjs.end(), "Project not found.");

        auto theprj = surpriseprjs.get(project_id);
        surpriseprjs.modify(iterator, author, [&](auto& surpriseprj) {
            surpriseprj.items.find(item_id).push_back(winner);
        });
    }
}
