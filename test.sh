# bash scripts used while testing contracts

# 0.0 edit the config.ini: ~/Library/Application Support/eosio/nodeos/config to enable plugins of eos private node you are about to start and 
nodeos
#      or 
# 0.1 Start the nodeos with  
nodeos -e -p eosio --plugin eosio::producer_plugin --plugin eosio::chain_api_plugin --plugin eosio::http_plugin --plugin eosio::history_api_plugin  --contracts-console --verbose-http-errors

# 1. Create new wallet with the name "cdg"(or whatever name you like) for this project
cleos wallet create -n cdg --to-console
##// my key(for test only): PW5KfKXy8qXD3JbKZ3RosqS6PYfm9eBjx2mHveVm4T4dfz7d4LrFE
cleos wallet unlock -n cdg

# 2. Generate two pair of keys (use the command twice for an account OwnerKey and ActiveKey 2-level authority)
cleos create key --to-console
cleos create key --to-console
## my Private key: 5KamKnnmFZD1pBLKmGweoVafFkxvr9R6xPYbKLwEoEviVcivBWt
## my Public key: EOS5rhWHPySUdtDTPWMkqiatv3xgU2EPwKnjSe4Vr3eYFUA1dPPeB
## my Private key: 5JurHoxQEbRtgKeeb7ZN28PECn92wJrgVSrP4f3dyPHxMY1NABC
## my Public key: EOS8XjjYgLcUkBbHYSacgsx2CRnbToApiwkNoaEk5ft9FfMxWgYJj
cleos create key --to-console
cleos create key --to-console
# Private key: 5K9U98d63o1z9YXXgBE8uCVvF7co8Tdp6VoywKhv9bg4N3iokxQ
# Public key: EOS6Lg1zGGdtrf5hAihherwSnaM3QeGdwQTj9MZPv2BAMwbBXX2rf
# Private key: 5KjiKSVyFEnR8rr2TmxMZBDSrvp45LVX9BSyseEFsVBtruaGGXg
# Public key: EOS5KA5u5aURzcrqjX4BXfVdbaYWVLJgHjD4Cu42Qh3vqoggEvbhB
cleos create key --to-console
cleos create key --to-console
# Private key: 5Jkiafdf6wNA9QB6AuvtGg4bQNfdnPmW45VUL3DSfKyxbT3yDCd
# Public key: EOS6AtkW85adJTEcDoSJs6kt9z9AJYYUQcKDoChAPFLiFnH12ZMqA
# Private key: 5Jz56BYswoqEHQ21zhrkeiSNqFADaMyR7jfPtyW2tatnzaUBZo5
# Public key: EOS7iqThxC1QKc7ga9ARvRJZm9VDoQs17Eydj13kYzENMbsMgrmHU
cleos create key --to-console
cleos create key --to-console
# Private key: 5K28iTySXjPCDp1rv45ikZEPDizRa8nitXhUPuvTCv9e7yc41Z7
# Public key: EOS5TSx8A2S6H5AqYvaJoGvahF47i7eMN53RNdpRHN5Bpy4xWEHY9
# Private key: 5KUm9Z2LQtZNDmudvcYuZnoCipbsTgUh8EoPcMCbyAJzsujeGTT
# Public key: EOS7at6Lxa2WZsoHD3bxryqZrQgTbt78Mh68G3YnoXAJBDZLh2hZ8
cleos create key --to-console
cleos create key --to-console
# Private key: 5JdftsSpDz8tooCRLk5Pg5Kixu3cCaConwRSc3iu2JhFjeED27b
# Public key: EOS6FavWio4KskbAk8cj2HCWT8cTTi3ExHmT8iSaJrLjtJjiUyCUE
# Private key: 5JDJ5LbtB1SwFFzgWEqesfQXWZT4yxm2vMVeSXYJUpcs9S32W2W
# Public key: EOS5Ucryr5oEsMMSHu2v7JzXZCf2jU7TLdUGJLyofvf81zoUaAPUp

# 3. Import the generated private keys in the wallet (you need to specify the wallet)
cleos wallet import -n cdg 
cleos wallet import -n cdg 
cleos wallet import -n cdg 
cleos wallet import -n cdg 
cleos wallet import -n cdg 

# 4. Add the private key of the "eosio" into your wallet
# Note: If you're receiving an Error 3090003: Provided keys, permissions, and delays do not satisfy declared authorizations
# you probably should add the private key of the "eosio" account to your wallet. 
cleos wallet import --private-key=5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3 -n cdg

# 5. Create the account using the public keys (will be destoryed if nodeos restart)
# for contract account:
cleos create account eosio pa EOS5rhWHPySUdtDTPWMkqiatv3xgU2EPwKnjSe4Vr3eYFUA1dPPeB EOS8XjjYgLcUkBbHYSacgsx2CRnbToApiwkNoaEk5ft9FfMxWgYJj
# for test accounts:
cleos create account eosio vanel EOS6Lg1zGGdtrf5hAihherwSnaM3QeGdwQTj9MZPv2BAMwbBXX2rf EOS5KA5u5aURzcrqjX4BXfVdbaYWVLJgHjD4Cu42Qh3vqoggEvbhB
cleos create account eosio ralph EOS6AtkW85adJTEcDoSJs6kt9z9AJYYUQcKDoChAPFLiFnH12ZMqA EOS7iqThxC1QKc7ga9ARvRJZm9VDoQs17Eydj13kYzENMbsMgrmHU
cleos create account eosio stevejobs EOS5TSx8A2S6H5AqYvaJoGvahF47i7eMN53RNdpRHN5Bpy4xWEHY9 EOS7at6Lxa2WZsoHD3bxryqZrQgTbt78Mh68G3YnoXAJBDZLh2hZ8
cleos create account eosio jackma EOS6FavWio4KskbAk8cj2HCWT8cTTi3ExHmT8iSaJrLjtJjiUyCUE EOS5Ucryr5oEsMMSHu2v7JzXZCf2jU7TLdUGJLyofvf81zoUaAPUp

# 6. Check these accounts: cleos get account ACCOUNTNAME –json(-j)
cleos get account pa -j

# 7. Compile the Surprise contract (make sure you are currently in the project path, or you need to use absolute path)
cd /Users/treasersmac/programming/EOS/Dapps/Coup_de_Grace/contract/Surprise
eosio-cpp -o Surprise.wasm Surprise.cpp --abigen


# 8. Deploy the Suprise contract
cleos set contract pa /Users/treasersmac/programming/EOS/Dapps/Coup_de_Grace/contract/Surprise -p pa@active


# 9. Test the Suprise contract

# test Suprise::showinfo action
cleos push action pa showinfo '[]' -p vanel@active 
# or:
cleos push action pa showinfo '[]' -p vanel@active --json

# test Suprise::create action
cleos push action pa create '["vanel","1","testlottery"]' -p vanel@active
cleos push action pa create '["vanel","2","yearconf"]' -p vanel@active
cleos push action pa create '["vanel","3","yearconf2"]' -p vanel@active
# test Suprise::checkbyid action
cleos push action pa checkbyid '["vanel","1"]' -p vanel@active
cleos push action pa checkbyid '["vanel","2"]' -p vanel@active
cleos push action pa checkbyid '["vanel","3"]' -p vanel@active
# test Suprise::checkn action
cleos push action pa checkn '["vanel","2"]' -p vanel@active 
# test Suprise::additem action( a lottery project may have many items for different level of prize )
cleos push action pa additem '["vanel","1","2","iPhone XS","2","25"]' -p vanel@active # 2 out of max 25 wins the prizes
# test Suprise::addcad action
cleos push action pa addcad '["vanel","1","2","007"]' -p vanel@active
cleos push action pa addcad '["vanel","1","2","stevejobs"]' -p vanel@active
cleos push action pa addcad '["vanel","1","2","jackma"]' -p vanel@active
cleos push action pa addcad '["vanel","1","2","billgates"]' -p vanel@active
cleos push action pa addcad '["vanel","1","2","wuuzhaohui"]' -p vanel@active
cleos push action pa addcad '["vanel","1","2","jaychou"]' -p vanel@active
# test Suprise::activate action
cleos push action pa activate '["vanel","1","2"]' -p vanel@active
cleos push action pa checkbyid '["vanel","1"]' -p vanel@active
cleos push action pa checkn '["vanel","3"]' -p vanel@active

cleos push action pa additem '["vanel","1","3","Tesla model S","1","25"]' -p vanel@active
cleos push action pa addcad '["vanel","1","3","satoshi"]' -p vanel@active
cleos push action pa addcad '["vanel","1","3","vitalik"]' -p vanel@active
cleos push action pa addcad '["vanel","1","3","bytemaster"]' -p vanel@active
cleos push action pa activate '["vanel","1","3"]' -p vanel@active
cleos push action pa checkbyid '["vanel","1"]' -p vanel@active
cleos push action pa checkn '["vanel","3"]' -p vanel@active


# Above finish all basic and pass test
# Below will be some normal small updates


# bugs unsolved:
# 1. a specifiy item can be activate many times...
#    fixed!!
# 2. if winumber is bigger than number of candidates...
#    fixed!
cleos push action pa additem '["vanel","1","1","XiaoMi Phone","5","25"]' -p vanel@active # 5 out of 25
cleos push action pa checkbyid '["vanel","1"]' -p vanel@active
cleos push action pa addcad '["vanel","1","1","18258583270"]' -p vanel@active 
cleos push action pa activate '["vanel","1","1"]' -p vanel@active 
cleos push action pa checkn '["vanel","3"]' -p vanel@active