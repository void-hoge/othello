# othello
othello-programming
# how to build and run
## build  
開発はclang, gccで行なっています。c++11以上に対応したコンパイラが必要です。makefileは-std=c++17で書いています。  
先手後手を入れ替えるには、main.cppの中身、
```
// gm.sente();
gm.aivsai();
// gm.gote();
```
を  
```
gm.sente();
// gm.aivsai();
// gm.gote();
```
みたいな感じで書き換えるといけます。  

`$ make`
## run
`$ make run`  
手は`(縦座標) (横座標)`みたいな感じで入力します。  
数字の間はスペースを開けてください。  
