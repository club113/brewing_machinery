说明：
BrewBeer：出厂程序，程序地址：0X08000000 一般不会更改 运行当前程序时如果有升级命令则会升级APP1
BrewBeer_APP_1：升级程序1  程序地址 0X08012000  运行当前程序时如果有升级命令则会升级APP2
BrewBeer_APP_2：升级程序1  程序地址 0X08024000  运行当前程序时如果有升级命令则会升级APP1


当前程序区分：
BrewBeer：RUNLED 闪烁
BrewBeer_APP_1：RUNLED ，COM1LED，COM2LED 闪烁
BrewBeer_APP_2：COM1LED 闪烁
new