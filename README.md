# 实现功能：
1.开奖方式：手动开奖
2.是否需要签名
3.创建完以后输入二维码
4.项目状态：待开奖、以结束
5.抽奖名单审核

具体实现
## 智能合约：
抽奖完成后接受到抽奖者的数据：
抽奖项目{奖项，获得者1的手机，获得者i的手机}
存到区块链。
### 解决了随机数
使用了基于区块数据的随机数产生算法：
### 解决了Multi_index的table下的vector的元素中的vector的元素的压栈
```surpriseprjs.modify(iterator, author, [&](auto& surpriseprj) {
    surpriseprj.items[item_id-1].winners.push_back(winner);
});```

## 客户端：

加入抽奖、创建抽奖、查看抽奖历史三大功能

加入抽奖{
你要参与的抽奖项目名：
你的联系方式手机：
你的姓名：
你的验证信息（学号）：
}
创建抽奖{
分类：单方抽奖、交互抽奖
你要创建的抽奖项目名：
奖项数目：i
奖项1{名称；人数}
...
奖项i{名称；人数}
从待抽奖者池中随机选择进行抽奖。
显示抽奖结果。
抽完后选择上传到区块链或者放弃重抽。
单方抽奖待抽奖者池为数字，
交互抽奖待抽奖者池为用户的手机号。
}
