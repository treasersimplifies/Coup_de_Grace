
// scripts which are the same in index.html
var info="";
        privateKey_pa   ="PW5KfKXy8qXD3JbKZ3RosqS6PYfm9eBjx2mHveVm4T4dfz7d4LrFE";
        // defaultPrivateKey="5KjiKSVyFEnR8rr2TmxMZBDSrvp45LVX9BSyseEFsVBtruaGGXg";
        vanelOwnerKey   ="5K9U98d63o1z9YXXgBE8uCVvF7co8Tdp6VoywKhv9bg4N3iokxQ";
        vanelActiveKey  ="5KjiKSVyFEnR8rr2TmxMZBDSrvp45LVX9BSyseEFsVBtruaGGXg";
        const rpc = new eosjs_jsonrpc.default('http://127.0.0.1:8888');
        const signatureProvider = new eosjs_jssig.default([vanelActiveKey]);
        const api = new eosjs_api.default({ rpc, signatureProvider });
        function pushAction(actionName, dataValue){
            (async () => {
                try {
                    const result = await api.transact({
                        actions: [{
                            account: 'pa',
                            name: actionName,
                            authorization: [{
                                actor: 'vanel',
                                permission: 'active',
                            }],
                            data: dataValue,
                        }]
                        }, {
                        blocksBehind: 3,
                        expireSeconds: 30,
                    });
                    // alert(JSON.stringify(result, null, 2));
                    var transaction_id = result.transaction_id;
                    var console_output = result.processed.action_traces[0].console;
                    if(actionName=="checkn"||actionName=="checkbyid"){
                        //operate_output作用：英文替换为中文。
                        var operate_output = console_output.replace(/Id/g,"<strong>主奖项编号</strong>");
                        operate_output = operate_output.replace(/Name/g,"<strong>奖项名称</strong>");
                        operate_output = operate_output.replace(/Items: /g,"");
                        operate_output = operate_output.replace(/id:/g,"  子奖项编号：");
                        operate_output = operate_output.replace(/context:/g,"  内容：");
                        operate_output = operate_output.replace(/winners' number:/g,"  获奖人数：");
                        operate_output = operate_output.replace(/winners:/g,"  获奖者：");
                        operate_output = operate_output.replace(/cadidates' number:/g,"  参与者人数：");
                        //显示在blockchain_output界面，查询显示记录
                        surprise_prjs = console_output.split("|||");
                        //显示在operate_output界面，查询显示记录
                        surprise_prjs_operate = operate_output.split("|||");
                        var prjs_num = surprise_prjs.length;
                        let blockchainText = "";
                        let operateText = "";
                        // document.getElementById("blockchain_output").innerHTML = console_output;
                        for(var i=1; i<prjs_num; i++){//对每个主奖项：
                            //对总体的blockchain的console输出进行分解，分解成一个一个主奖项：
                            blockchainText += "<p>" + surprise_prjs[i] + "</p>";
                            // var surprise_items = surprise_prjs[i].split("||-");
                            operateText += "<p style='border-top:2px'>";
                            //对每个主奖项进行分解，分解成为Name 和 Items：
                            var surprise_items_operate = surprise_prjs_operate[i].split("||-");
                            for(var j=0; j<surprise_items_operate.length; j++){
                                if(j>=1){//对于Items：
                                    //将Items分解：i代表第i个主奖项，j必定==2，代表Items
                                    var items = surprise_items_operate[j].split("||");
                                    operateText += items[0];//"Items:"
                                    for(var k=1;k<items.length;k++){ //k 代表第k个item
                                        operateText += "<p>" +"&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp";
                                        var iteminfo = items[k].split("|-");
                                        operateText += iteminfo[0] +"</p>"+"<p>"+"&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp&nbsp"+ iteminfo[1] ;
                                    }
                                
                                }else{//对于Id和Name：
                                    operateText += surprise_items_operate[j];
                                }
                            }
                            operateText += "</p>";
                        }
                        operateText = operateText.replace(/Undefined/g,"暂未设置");
                        operateText = operateText.replace(/undefined/g,"");
                        operateText = operateText.replace(/not generated yet/g,"未产生");
                        document.getElementById("blockchain_output").innerHTML = blockchainText;
                        document.getElementById("operate_output").innerHTML = operateText;
                    }else if(actionName=="showinfo"){
                        var prj_totalnum = console_output.indexOf(":");
                        document.getElementById("limit").innerHTML = "(目前链上所存主奖项共有：" + console_output[prj_totalnum+1]+")";
                        document.getElementById("blockchain_output").innerHTML = console_output;
                    }
                    else{
                        document.getElementById("transaction_hash").innerHTML="结果已上链！本次交易哈希为 "+transaction_id+", 请点击查看结果以示结果！";
                        document.getElementById("blockchain_output").innerHTML = console_output;
                    }
                    
                } catch (e) {
                    var err_info = '\nCaught exception: ' + e;
                    if (e instanceof eosjs_jsonrpc.RpcError)
                        err_info += '\n' + JSON.stringify(e.json, null, 2);
                    alert(err_info);
                    // throw(err);
                }
            })();
        }
        function showInfo(){
            pushAction("showinfo",{});
        }
        function checkN(){
            var checkNumberValue = document.getElementById("check_number").value;
            pushAction("checkn",{author:"vanel", number:checkNumberValue});
        }
        function checkById(){
            var idValue = document.getElementById("check_id").value;
            pushAction("checkbyid",{author:"vanel", project_id:idValue});
        }
        function addCad(){
            var idValue = document.getElementById("cad_prj_id").value;
            var itemId = document.getElementById("cad_item_id").value;
            var nameValue = document.getElementById("cad_name").value;
            pushAction("addcad",{author:"vanel", project_id:idValue, item_id:itemId, cadname:nameValue});
        }
        function activate(){
            var prjId = document.getElementById("activate_prj_id").value;
            var itemId = document.getElementById("activate_item_id").value;
            pushAction("activate",{author:"vanel", project_id:prjId, item_id:itemId})
        }
        function addItem(){
            var prjId = document.getElementById("item_prj_id").value;
            var itemId = document.getElementById("item_id").value;
            var itemName = document.getElementById("item_context").value;
            var winNumber = document.getElementById("item_winnumber").value;
            var maxNumber = document.getElementById("item_maxnumber").value;
            pushAction("additem",{author:"vanel", project_id:prjId, item_id:itemId, item_name:itemName, winumber:winNumber, maxnumber:maxNumber});
        }
        function create(){
            var prjId = document.getElementById("create_id").value;
            var prjName = document.getElementById("create_name").value;
            pushAction("create",{author:"vanel", project_id:prjId, project_name:prjName});
        }
        function showExample(){
            document.getElementById("operate_output").innerHTML = 
                "# test Suprise::create action <br />" +
                "create vanel,1,testlottery <br/>" +
                "create vanel,2,yearconf <br/>" +
                "create vanel,3,yearconf2 <br/>" +
                "# test Suprise::checkbyid action <br/>" +
               " checkbyid vanel,1 <br/>" +
               " checkbyid vanel,2 <br/>" +
                "# test Suprise::checkn action <br/>" +
                "checkn vanel,2 <br/>" +
                "# test Suprise::additem action  <br/>" +
                "additem vanel,1,1,iPhone XS,2,25 <br/>" +
                "# test Suprise::addcad action <br/>" +
                "addcad vanel,1,1,007 <br/>" +
                "addcad vanel,1,1,stevejobs <br/>" +
                "addcad vanel,1,1,jackma <br/>" +
                "addcad vanel,1,1,billgates <br/>" +
                "addcad vanel,1,1,wuuzhaohui <br/>" +
                "pa addcad vanel,1,1,jaychou <br/>" +
                "# test Suprise::activate action <br/>" +
                "activate vanel,1,1 <br/>" +
                "checkbyid vanel,1 <br/>" +
                "checkn vanel,3 <br/>" +
                "<br/>" +
                "additem vanel,1,3,Tesla model S,1,25 <br/>"+
                "addcad vanel,1,3,satoshi <br/>" +
                "addcad vanel,1,3,vitalik <br/>" +
                "addcad vanel,1,3,bytemaster <br/>" +
                "activate vanel,1,3 <br/>" +
                "checkbyid vanel,1 <br/>" +
                "heckn vanel,3 <br/>";
            
        }