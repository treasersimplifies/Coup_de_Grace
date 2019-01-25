# Coup_ de_Grace
The distinguished ability of PA(Phantom Assassin)

Phantom Assassin refines her combat abilities, gaining a chance of delivering a devastating critical strike to enemy units. Stifling Dagger shares the same critical strike chance.

# 要求
搭建私有链，设想某个去中心化应用场景，编写智能合约，完成智能合约的编译、部署和调用。基于任意架构的区块链平台开发一款可运行的DApp。
### 提交内容
1. 完整源代码
2. word说明文档（包括：）
	* 开发环境搭建
	* 功能设计
	* 合约设计
	* 系统实现和部署
	* 主要界面截图与说明
	
# 2 功能概述(Overview)
This DApp achieves:

1. Can create the Main Awards and add sub-awards under the main awards. For example, you can create the main prize of the "Association Activity Draw", and then add "First Prize" and "Second Prize" sub-scores to it. After creation, the transaction hash value for creating a award will be displayed.
2. Can add unilateral award participants, specifically to sub-awards. For example, adding 10 participants to participate in the "second prize".
3. Can unilaterally award a sub-award. And the results of the lottery will be automatically put on blockchain.
4. Can query the history. You can query by the main award ID as the index value, or you can query the first **n** records in the history.


Chinese:

1. 能实现创建抽奖主奖项，并在主奖项下添加子奖项。比如创建“协会活动抽奖”这个主奖项，并在下面添加“一等奖”、“二等奖”等子奖项。创建后显示创建奖项的交易哈希值。
2. 能单方面添加奖项参与者，具体到子奖项。比如添加10位参与抽“二等奖”。
3. 能单方面对某一子奖项开奖。并将抽奖结果自动上链。
4. 能查询历史记录。可以按主奖项ID为索引值进行查询，也可以查询历史中前n个记录。

# 3 智能合约(Contract)
EOS的智能使用C++开发（也可以使用其他语言），限于时间所限，第一版抽奖软件比较简单，只需要一个[Surprise](https://github.com/treasersimplifies/Coup_de_Grace/tree/master/contract/Surprise)合约就能实现既定功能。

在这个合约里定义/实现了如下内容：(The following are defined in contracts :)
### actions

* showinfo:用于显示合约相关信息。       ```void showinfo();```
* create:用于新建主奖项(create a main reward)。      ```void create(const account_name author, uint64_t project_id, string& project_name);```
* additem:用于向某个主奖项下添加子奖项(add sub-reward)。    ```void additem(const account_name author, uint64_t project_id, uint64_t item_id, string& item_name, uint32_t winumber, uint32_t maxnumber);```
* addcad:用于向某主奖项下的子奖项添加参与者(add candidates)。    ```void addcad(const account_name author, uint64_t project_id, uint64_t item_id, string& cadname);```
* activate:用于针对某主奖项下的子奖项的开奖(get lottery done)。   ```void activate(const account_name author, uint64_t project_id, uint64_t item_id);```
* checkbyid:用于查询奖项记录(query reward by id)。   ```void checkbyid(const account_name author, uint64_t project_id);```
* checkn:用于查询奖项记录(query n records)。      ```void checkbyid(const account_name author, uint64_t project_id);```

### 数据结构(data structure)

1. 子奖项结构(sub-reward structure)

```
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
```

2. 主奖项结构(main structure)

```
struct surpriseprj{
    uint64_t id;
    string name;
    vector<surpriseitem> items;//
    
    uint64_t primary_key() const { return id; }
    // EOSLIB_SERIALIZE(surpriseprj, (id)(name))
};         
```
### Multi_Index
定义了一个Multi_Index的table：

```typedef multi_index<N(surpriseprj), surpriseprj> surpriseprjIndex;```

然后是关于这个table的增、改、查操作等：(operations defined for this table)

```
surpriseprjIndex surpriseprjs(_self, _self);
//增：add
surpriseprjs.emplace(author, [&](auto& surpriseprj) {
    surpriseprj.id = project_id;
    surpriseprj.name = project_name;
});
//改：update
surpriseprjs.modify(iterator, author, [&](auto& surpriseprj) {
    surpriseprj.items.push_back(surpriseitem{
        item_id,
        item_name,
        winumber,
        maxnumber
    });
});
//查：query
auto theprj = surpriseprjs.get(project_id);
print("||| Id: ", theprj.id);
print(" ||- Name: ", theprj.name.c_str());
print(" ||- Items: ");    
```

### 随机数(random number)
EOS无法使用```#include<random>```，所以需要寻找其他的随机源。

参考了(referrence)：[generEOS/eosio.random](https://github.com/generEOS/eosio.random) and [Randomization in Contracts](https://developers.eos.io/eosio-cpp/docs/random-number-generation)

原理是：通过区块数据来作为种子，产生哈希，比较不同的哈希，得出比较结果，把比较结果作为随机数。(use block data as the seed)

```
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
```

### Multi_index的table下的vector的元素中的vector的元素的压栈

```
surpriseprjs.modify(iterator, author, [&](auto& surpriseprj) { 
	surpriseprj.items[item_id-1].winners.push_back(winner);
});
``` 

# 4 前端(Frontend)
前端界面如下：
![OverView](frontend/src/OverView2.png)
因为前端比较简单，没有用什么框架或者前端库，所有内容直接写在一个html文件中了,需要引入如下文件，都位于eosjs库中（eosjs库的引入见下“5 系统实现和部署”）：


```
<script src='eosjs/dist-web/eosjs-api.js'></script>
<script src='eosjs/dist-web/eosjs-jsonrpc.js'></script>
<script src='eosjs/dist-web/eosjs-jssig.js'></script>
```
## 4.1 界面(UI)
前端的界面：分三部分，**顶部显示区域**、**中间按钮区域**、**底部查询显示区域**。

### 顶部显示区域
将与智能合约进行交互得到的输出作简单处理后显示在此。

而其中的蓝色方框内能显示对抽奖项目进行增、改操作时的交易哈希值。

### 中间按钮区域
每个按钮都一一、严格地对应一个合约中的action，而按钮旁边的输入框都是用来逐个输入逐个action需要的参数的。原理很简单。

### 底部查询显示区域
进行查操作（按下查询按钮）后显示记录的区域。

**注意：查询前最好按下“显示信息”按钮来看看链上有多少个抽奖记录，不能越界。**
## 4.2 主要代码(Core Code)
EOS提供了eosjs和eosjs2两个JavaScript库，能用来对交易进行签名和发送交易等操作。这里我使用的是eosjs。前端中的核心代码是利用EOS提供的JavaScript API和区块链节点建立通信（包括签名）和向智能合约推送action的部分。

### 建立通信

```
vanelActiveKey  ="5KjiKSVyFEnR8rr2TmxMZBDSrvp45LVX9BSyseEFsVBtruaGGXg";
const rpc = new eosjs_jsonrpc.default('http://127.0.0.1:8888');
const signatureProvider = new eosjs_jssig.default([vanelActiveKey]);
const api = new eosjs_api.default({ rpc, signatureProvider });
```
这种写法非常简单，不合适在实际生产开发中使用，因为不安全。

### 推送action

```
function pushAction(actionName, dataValue){
    (async () => {
        try {
            const result = await api.transact({
                actions: [{
                    account: 'pa', //部署合约的账号
                    name: actionName,
                    authorization: [{
                        actor: 'vanel', //本客户端拥有私钥进行签名的账号
                        permission: 'active',
                    }],
                    data: dataValue,
                }]
                }, {
                blocksBehind: 3,
                expireSeconds: 30,
            });
            // alert(JSON.stringify(result, null, 2));
            // 返回的result是一个JSON对象
            var transaction_id = result.transaction_id;
            var console_output = result.processed.action_traces[0].console;    
            ...... //略去对从合约收到的文本进行进一步处理的内容。                
        } catch (e) {
            var err_info = '\nCaught exception: ' + e;
            if (e instanceof eosjs_jsonrpc.RpcError)
                err_info += '\n' + JSON.stringify(e.json, null, 2);
            alert(err_info);
        }
    })();
}
```
实现这个推送action的函数后，之后的各种action推送都只要调用它就行，不需要自己重新针对每个action写一个函数。

# 5 系统实现和部署(Integrate and Deploy)
## 系统集成
编写完合约和前端后进行集成，项目结构：

```
Coup_de_Grace								
	  |-contract							  
	  		|-Surprise					       
	  			  |-Surprise.hpp
	  			  |-Surprise.cpp
	  			  |-Surprise.abi
	  			  |-Surprise.wasm
	  |-frontend
	  		|-index.html
	  		|-index.css
	  		|-eosjs
	  		|-src
	  |-README.md
	  |-test.sh
```
如果没有eosjs，则需要自己引入：

```
# 在此之前自然要装好node.js和npm
# in test.sh:
cd ${DappsDIR}/frontend
git clone https://github.com/EOSIO/eosjs.git 
cd eosjs
npm install
npm run build-web
```
## 部署准备工作
部署前需要启动本地EOS单点节点（相当于以太坊私有链），然后创建好测试本项目要使用的钱包、导入账户密钥、创建账户（这些过程在test.sh中都有）：

```
# in test.sh:
# 0.0 edit the config.ini: ~/Library/Application Support/eosio/nodeos/config 
# to enable plugins of eos private node you are about to start and:
nodeos
#      or 
# 0.1 Start the nodeos with :
nodeos -e -p eosio --plugin eosio::producer_plugin --plugin eosio::chain_api_plugin --plugin eosio::http_plugin --plugin eosio::history_api_plugin  --contracts-console --verbose-http-errors --enable-stale-production --max-transaction-time 3000
```
是的，你确实需要激活这么多插件，否则你无法测试本项目了...比如：

```http_plugin --plugin ```是为了激活RPC API

```--contracts-console```是为了激活控制台输入，如果不激活，那你向智能合约发生action时你的命令行里就没有输出了

```--max-transaction-time 3000```是为了提高交易执行超时时间，默认是30ms，因为产生随机数需要不少时间（相对于程序其他部分而言），所以如果一次性随机抽奖的人数过多就会超时，导致交易无法执行。

这么多插件、参数的必要性都是在我尝试了很多次以后通过经验再加上EOSIO官网的文档得出的。建议通过编辑config.ini文件来修改这些参数，这样你就默认使能这些东西了。其他的插件说明见：[EOSIO nodeos plugins](https://developers.eos.io/eosio-nodeos/docs/history_api_plugin)


```
# in test.sh:
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
```

**注意：**由于我是做测试使用，所以直接把私钥打印到了命令上，实际写DApp时不要使用此选项，应选择把私钥打印到某个文件里。

```
# in test.sh:
# 3. Import the generated private keys in the wallet (you need to specify the wallet)
cleos wallet import -n cdg 
cleos wallet import -n cdg 
cleos wallet import -n cdg 
cleos wallet import -n cdg 

# 4. Add the private key of the "eosio" into your wallet
# Note: If you're receiving an Error 3090003: Provided keys, permissions, and delays do not satisfy declared authorizations
# you probably should add the private key of the "eosio" account to your wallet. 
cleos wallet import --private-key=5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3 -n cdg
```

**注意：**eosio的私钥必须导入，因为要创建的所有账户都是通过eosio产生的。不导入它的私钥，没办法来创建账户。

```
# in test.sh:
# 5. Create the account using the public keys (will be destoryed if nodeos restart)
# for contract account:
cleos create account eosio pa EOS5rhWHPySUdtDTPWMkqiatv3xgU2EPwKnjSe4Vr3eYFUA1dPPeB EOS8XjjYgLcUkBbHYSacgsx2CRnbToApiwkNoaEk5ft9FfMxWgYJj
# for test accounts:
cleos create account eosio vanel EOS6Lg1zGGdtrf5hAihherwSnaM3QeGdwQTj9MZPv2BAMwbBXX2rf EOS5KA5u5aURzcrqjX4BXfVdbaYWVLJgHjD4Cu42Qh3vqoggEvbhB

# 6. Check these accounts: cleos get account ACCOUNTNAME –json(-j)
cleos get account pa -j
```

## 部署
做好这些准备工作后就可以来部署合约到EOS区块链上了。

```
# in test.sh:
# 7. Compile the Surprise contract (make sure you are currently in the project path, or you need to use absolute path)
cd /Coup_de_Grace/contract/Surprise
eosio-cpp -o Surprise.wasm Surprise.cpp --abigen

# 8. Deploy the Suprise contract
cleos set contract pa /Coup_de_Grace/contract/Surprise -p pa@active

```
如果没出现错误就完成部署了。

## 使用的命令
以上我们使用到了很多EOS的命令，主要有：nodeos, eosio-cpp, cleos.

其中nodeos是与节点有关的命令，在本项目中主要用来启动区块链单节点，其详细内容见：[EOSIO nodeos](https://developers.eos.io/eosio-nodeos/docs/overview-1)

eosio-cpp 是把cpp文件编译为wasm文件的命令。EOS使用WebAssembly对智能合约的C++代码进行编译和执行，其核心原因是WebAssembly性能好。WebAssembly就像是一个编译器能让EOS看懂C/C++代码。

cleos是与区块链进行交互的主要命令，它包含很多子命令（仅介绍本项目中用到的）：

1. cleos wallet: 与钱包相关的命令，本项目中用到了创建钱包（create）、解锁钱包（unlock）、导入密钥（import）。

	```
	Usage: cleos wallet SUBCOMMAND
	Subcommands:
	  create                      Create a new wallet locally
	  open                        Open an existing wallet
	  lock                        Lock wallet
	  lock_all                    Lock all unlocked wallets
	  unlock                      Unlock wallet
	  import                      Import private key into wallet
	  remove_key                  Remove key from wallet
	  create_key                  Create private key within wallet
	  list                        List opened wallets, * = unlocked
	  keys                        List of public keys from all unlocked wallets.
	  private_keys                List of private keys from an unlocked wallet in wif or PVT_R1 format.
	  stop                        Stop keosd (doesn't work with nodeos).
	```
	
2. cleos create: 创建命令，本项目中用到了创建密钥对（key）、创建账户（account）。

	```
	Usage: cleos create SUBCOMMAND
	Subcommands:
	  key                         Create a new keypair and print the public and private keys
	  account                     Create a new account on the blockchain (assumes system contract does not restrict RAM usage)
	```
	
3. cleos get: 获取命令，本项目只用到了获取账户信息（account）。

	```
	Usage: cleos get SUBCOMMAND
	Subcommands:
	  info                        Get current blockchain information
	  block                       Retrieve a full block from the blockchain
	  account                     Retrieve an account from the blockchain
	  code                        Retrieve the code and ABI for an account
	  abi                         Retrieve the ABI for an account
	  table                       Retrieve the contents of a database table
	  scope                       Retrieve a list of scopes and tables owned by a contract
	  currency                    Retrieve information related to standard currencies
	  accounts                    Retrieve accounts associated with a public key
	  servants                    Retrieve accounts which are servants of a given account 
	  transaction                 Retrieve a transaction from the blockchain
	  actions                     Retrieve all actions with specific account name referenced in authorization or receiver
	  schedule                    Retrieve the producer schedule
	  transaction_id              Get transaction id given transaction object
	```
	
4. cleos set: 部署命令，本项目只用到了部署合约（contract）。

	```
	Usage: cleos set [OPTIONS] SUBCOMMAND
	
	Options:
	  -h,--help                   Print this help message and exit
	
	Subcommands:
	  code                        Create or update the code on an account
	  abi                         Create or update the abi on an account
	  contract                    Create or update the contract on an account
	  account                     set or update blockchain account state
	  action                      set or update blockchain action state
	```

5. cleos push: push命令，本项目只用到了push action。

	```
	Usage: cleos push SUBCOMMAND
	Subcommands:
	  action                      Push a transaction with a single action
	  transaction                 Push an arbitrary JSON transaction
	  transactions                Push an array of arbitrary JSON transactions
	```

6. cleos的全部命令：
	
	```
	Usage: cleos [OPTIONS] SUBCOMMAND
	
	Options:
	  -h,--help                   Print this help message and exit
	  -u,--url TEXT=http://127.0.0.1:8888/
	                              the http/https URL where nodeos is running
	  --wallet-url TEXT=unix:///Users/treasersmac/eosio-wallet/keosd.sock
	                              the http/https URL where keosd is running
	  -r,--header                 pass specific HTTP header; repeat this option to pass multiple headers
	  -n,--no-verify              don't verify peer certificate when using HTTPS
	  --no-auto-keosd             don't automatically launch a keosd if one is not currently running
	  -v,--verbose                output verbose actions on error
	  --print-request             print HTTP request to STDERR
	  --print-response            print HTTP response to STDERR
	
	Subcommands:
	  version                     Retrieve version information
	  create                      Create various items, on and off the blockchain
	  convert                     Pack and unpack transactions
	  get                         Retrieve various items and information from the blockchain
	  set                         Set or update blockchain state
	  transfer                    Transfer EOS from account to account
	  net                         Interact with local p2p network connections
	  wallet                      Interact with local wallet
	  sign                        Sign a transaction
	  push                        Push arbitrary transactions to the blockchain
	  multisig                    Multisig contract commands
	  wrap                        Wrap contract commands
	  system                      Send eosio.system contract action to the blockchain.
	```

# 6 测试与使用说明(Test Examples)

可以通过两种方式（两种类型的客户端）来测试智能合约：通过命令行（Cli）和通过Web端（JavaScript API）。
## 6.1 CLi 
这里我将用到的测试内容都写到test.sh中了。测试的内容比较简单。

```
# in test.sh:
# 9. Test the Suprise contract

# test Suprise::showinfo action
cleos push action pa showinfo '[]' -p vanel@active 
# or:
cleos push action pa showinfo '[]' -p vanel@active --json

# test Suprise::create action
cleos push action pa create '["vanel","1","testlottery"]' -p vanel@active
cleos push action pa create '["vanel","2","annualconf"]' -p vanel@active
cleos push action pa create '["vanel","3","annualconf2"]' -p vanel@active

# test Suprise::checkbyid action
cleos push action pa checkbyid '["vanel","1"]' -p vanel@active
cleos push action pa checkbyid '["vanel","2"]' -p vanel@active
cleos push action pa checkbyid '["vanel","3"]' -p vanel@active

# test Suprise::checkn action
cleos push action pa checkn '["vanel","2"]' -p vanel@active 

# test Suprise::additem action( a lottery project may have many items for different level of prize )
cleos push action pa additem '["vanel","1","1","iPhone XS","2","25"]' -p vanel@active # 2 out of max 25 wins the prizes

# test Suprise::addcad action
cleos push action pa addcad '["vanel","1","1","007"]' -p vanel@active
cleos push action pa addcad '["vanel","1","1","stevejobs"]' -p vanel@active
cleos push action pa addcad '["vanel","1","1","jackma"]' -p vanel@active
cleos push action pa addcad '["vanel","1","1","billgates"]' -p vanel@active
cleos push action pa addcad '["vanel","1","1","wuuzhaohui"]' -p vanel@active
cleos push action pa addcad '["vanel","1","1","jaychou"]' -p vanel@active
# test Suprise::activate action
cleos push action pa activate '["vanel","1","1"]' -p vanel@active
cleos push action pa checkbyid '["vanel","1"]' -p vanel@active
cleos push action pa checkn '["vanel","3"]' -p vanel@active

# integrated test:
cleos push action pa additem '["vanel","1","3","Tesla model S","1","25"]' -p vanel@active
cleos push action pa addcad '["vanel","1","3","satoshi"]' -p vanel@active
cleos push action pa addcad '["vanel","1","3","vitalik"]' -p vanel@active
cleos push action pa addcad '["vanel","1","3","bytemaster"]' -p vanel@active
cleos push action pa activate '["vanel","1","3"]' -p vanel@active
cleos push action pa checkbyid '["vanel","1"]' -p vanel@active
cleos push action pa checkn '["vanel","3"]' -p vanel@active
```

## Web端测试
Omitted here.

略。



# Futurn Plan
未来计划添加或改进的内容：

1. 对于密钥的操作应以更加安全的方式进行。
2. 客户端要新增“加入别人新建的抽奖项目按钮“。
3. 奖项创建完以后产生一串短数字或者二维码来方便其他人加入。奖项创建者不再需要自己来完成手动添加参奖者。
4. 但应该赋予奖项创建者抽奖名单审核的权力。
5. 参与抽奖者未来需要提供更多的信息。
6. 客户端能实现以主键（priject_id/item_id）以外的值进行历史记录查询。
7. 新建奖项不再需要手动提供id，直接输入名称、内容。
8. 对于访问越界、空内容的错误处理写得更详细。
9. 对于系统的其他错误处理需要写得更详细。
10. 对软件整体的测试更应充分。
11. 改进前端UI。