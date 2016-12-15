邮箱批量验证程序

V0.11
0.修正一些小错误；
1.input和output文件格式均修改为：
username||^^||password
行尾不要有空格


V0.10说明：
0.基于visual studio 2013编译；
1.程序当前目录下要有cfg.ini配置文件；
2.命令行程序，赞无UI；
3.命令说明：程序名 输入文件名
例:
命令行下输入 mailverifi.exe input.txt
程序读取input.txt内的邮箱账号信息进行批量验证，验证通过的将输出至out.txt

4.input.txt文件格式说明：
一行邮箱账号，一行邮箱密码格式

5.cfg.ini配置文件格式说明：
type = 163.com
server= smtp.163.com
encryption=0
port = 25

type--邮箱类型，即邮箱账号@后面的部分
server--smtp服务器域名
encryption--加密类型  0-无加密 1-TLS 2-SSL
port--smtp服务器端口

