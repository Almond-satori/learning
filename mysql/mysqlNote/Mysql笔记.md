# Mysql笔记,PART1

## 1.概述

1.数据库：DB，按照一定格式存储一些文件的组合。

2.数据库管理系统：DBMS，DBMS管理数据库中的数据，实现数据库数据的增删改查。

常见的DBMS：mysql，Oracle。SQLserver等

3.结构化查询语言：SQL，程序员编写SQL语句，DBMS执行SQL语句实现对数据库的增删改查

SQL：是一套标准的语言，在各种DBMS都可以使用



三者之间的关系：DBMS执行SQL实现对DB的增删改查操作



## 2.指令

管理员身份进入cmd

启动/关闭服务，在cmd中：

``` 
net start 服务名称
net stop 服务名称
```

登录数据库，在cmd中：

```
mysql -u root -p
输入密码
```

在mysql中：(注意mysql见分号才会执行)

``` 
//展示目录
show databases;
//使用目录中的数据库test
use test;
//创建新数据库
create database newdata;
//退出
exit;

//查看数据库下有哪些表
show tables;
//导入表
source 路径;
//查看数据库版本号
select version();
//查看当前使用的数据库
select database();
```

 进入某个数据库

```
use 数据库名
```



## 3.数据库的基本单元table

数据库中以表存储数据，表比较直观。

一张表中有行和列：

行：被称为数据/记录

列：被称为字段，如姓名字段，性别字段，年龄字段



## 4.SQL语言的分类

DQL:数据查询语言 select查询

DML:数据操作语言 insert，delete，update，操作具体数据

DDL:数据定义语言，凡是带有Create,drop,alter的都是DDL，DDL操作的是表的结构而不是具体数据

​		create:新增

​		drop:删除

​		alter:修改

TCL:事务控制语言

​		事务提交：commit

​		事务回滚：rollback

DCL:数据控制语言

​		授权：grant

​		撤销权限：revoke



## 5.简单查询

不区分大小写，且以分号结尾。

### 1.查询表

``` 
//只看表的结构，不看表的数据
desc 表名;

//查询所有字段,*方式实际开发中不建议，效率低，时间差
SELECT * FROM 表名;

//查询
SELECT 字段名 FROM 表名;

//查询多个字段
SELECT 字段名1,字段名2 FROM 表名;
```



### 2.给查询的列别名

```
select deptno as a1 ,dname as a2 from dept;
+----+------------+
| a1 | a2         |
+----+------------+
| 10 | ACCOUNTING |
| 20 | RESEARCH   |
| 30 | SALES      |
| 40 | OPERATIONS |
+----+------------+
```



使用as起别名，显示为别名展现出来，不会修改原来的表。select语句不会修改原来的表。

as可以省略

``` 
select deptno a1,dname a2 from dept;
```

若我们起的别名有空格，比如起名为a1 name

```
select dname a1 name from dept//error
```

必须加上' '，标准为单引号，双引号在Oracle中无法使用

```
 select dname 'a1 name' from dept;
+------------+
| a1 name    |
+------------+
| ACCOUNTING |
| RESEARCH   |
| SALES      |
| OPERATIONS |
+------------+
```



### 3.查询语句中字段使用数学表达式

```
select ename,sal*12 as '年薪' from emp;
+--------+----------+
| ename  | 年薪      |
+--------+----------+
| SMITH  |  9600.00 |
| ALLEN  | 19200.00 |
| WARD   | 15000.00 |
| JONES  | 35700.00 |
| MARTIN | 15000.00 |
| BLAKE  | 34200.00 |
| CLARK  | 29400.00 |
| SCOTT  | 36000.00 |
| KING   | 60000.00 |
| TURNER | 18000.00 |
| ADAMS  | 13200.00 |
| JAMES  | 11400.00 |
| FORD   | 36000.00 |
| MILLER | 15600.00 |
+--------+----------+
```



## 6.条件查询

 ### 1.基本格式

``` 
SELECT 字段
FROM 表名
WHERE 条件;
```

### 2.查询的条件

=  等于,在数据库中不能使用=null必须用is null，数据库中的null处在文件中就是什么也没有的，无法用=判断

``` 
select empno,sal from emp where ename='SMITH';//可以等于字符串
```

!=或者<>

<,<=,>=.>

between ... and ... 包含边界，必须左小右大，相当于>= num1 and <= num2

is null /is not null 为空的/不为空的



and 并且

or 或者

注意：and和or同时出现时存在优先级问题

查询工资大于2500并且部门编号为10或者20的员工：

 ``` 
 select * from emp where sal >2500 and deptno = 10 or deptno = 20;
 +-------+-------+-----------+------+------------+---------+------+--------+
 | EMPNO | ENAME | JOB       | MGR  | HIREDATE   | SAL     | COMM | DEPTNO |
 +-------+-------+-----------+------+------------+---------+------+--------+
 |  7369 | SMITH | CLERK     | 7902 | 1980-12-17 |  800.00 | NULL |     20 |
 |  7566 | JONES | MANAGER   | 7839 | 1981-04-02 | 2975.00 | NULL |     20 |
 |  7788 | SCOTT | ANALYST   | 7566 | 1987-04-19 | 3000.00 | NULL |     20 |
 |  7839 | KING  | PRESIDENT | NULL | 1981-11-17 | 5000.00 | NULL |     10 |
 |  7876 | ADAMS | CLERK     | 7788 | 1987-05-23 | 1100.00 | NULL |     20 |
 |  7902 | FORD  | ANALYST   | 7566 | 1981-12-03 | 3000.00 | NULL |     20 |
 +-------+-------+-----------+------+------------+---------+------+--------+
 ```



所有部门编号为20的员工都被找出了

可见sql语句表达的意思是，工资大于2500而且部门编号是10的员工或者部门编号是20的员工

证明了and优先级高于or



解决方法，加括号

``` 
select * from emp where sal >2500 and (deptno = 10 or deptno = 20);
+-------+-------+-----------+------+------------+---------+------+--------+
| EMPNO | ENAME | JOB       | MGR  | HIREDATE   | SAL     | COMM | DEPTNO |
+-------+-------+-----------+------+------------+---------+------+--------+
|  7566 | JONES | MANAGER   | 7839 | 1981-04-02 | 2975.00 | NULL |     20 |
|  7788 | SCOTT | ANALYST   | 7566 | 1987-04-19 | 3000.00 | NULL |     20 |
|  7839 | KING  | PRESIDENT | NULL | 1981-11-17 | 5000.00 | NULL |     10 |
|  7902 | FORD  | ANALYST   | 7566 | 1981-12-03 | 3000.00 | NULL |     20 |
+-------+-------+-----------+------+------------+---------+------+--------+
```



in：in(具体值1，具体值2，...)表示 = 具体1 or =具体值2 or...

具体值可以是字符串

not in 找出不是这些具体值的

```
select * from emp where sal in (800,5000);
+-------+-------+-----------+------+------------+---------+------+--------+
| EMPNO | ENAME | JOB       | MGR  | HIREDATE   | SAL     | COMM | DEPTNO |
+-------+-------+-----------+------+------------+---------+------+--------+
|  7369 | SMITH | CLERK     | 7902 | 1980-12-17 |  800.00 | NULL |     20 |
|  7839 | KING  | PRESIDENT | NULL | 1981-11-17 | 5000.00 | NULL |     10 |
+-------+-------+-----------+------+------------+---------+------+--------+
```



not取非



like:模糊查询，支持%和_匹配

%：表示任意多个字符

_：表示任意一个字符

```
select ename from emp where ename like '_a%';//name第二个字母是a的
select ename from emp where ename like '%o%';//name中有字母o的
select ename from emp where ename like '%t';//name中以t结尾的
```

若要寻找的值有下划线比如找jack_son

``` 
select ename from emp where ename like '%\_%';使用\转义字符
```

## 7.排序

### 1.基本结构

``` 
select ename,sal from emp order by sal desc;//降序
select ename,sal from emp order by sal asc;//升序
```

升序：

```
+--------+---------+
| ename  | sal     |
+--------+---------+
| SMITH  |  800.00 |
| JAMES  |  950.00 |
| ADAMS  | 1100.00 |
**| WARD   | 1250.00 |**
**| MARTIN | 1250.00 |**
| MILLER | 1300.00 |
| TURNER | 1500.00 |
| ALLEN  | 1600.00 |
| CLARK  | 2450.00 |
| BLAKE  | 2850.00 |
| JONES  | 2975.00 |
| FORD   | 3000.00 |
| SCOTT  | 3000.00 |
| KING   | 5000.00 |
+--------+---------+
```



### 2.多个排序条件

如果条件1 相同，按照条件2排序

``` 
 select ename,sal from emp order by sal asc, ename asc;
 +--------+---------+
| ename  | sal     |
+--------+---------+
| SMITH  |  800.00 |
| JAMES  |  950.00 |
| ADAMS  | 1100.00 |
**| MARTIN | 1250.00 |**
**| WARD   | 1250.00 |**
| MILLER | 1300.00 |
| TURNER | 1500.00 |
| ALLEN  | 1600.00 |
| CLARK  | 2450.00 |
| BLAKE  | 2850.00 |
| JONES  | 2975.00 |
| FORD   | 3000.00 |
| SCOTT  | 3000.00 |
| KING   | 5000.00 |
+--------+---------+
```



### 3.根据字段的位置排序

按照查询出的第二列排序：（不是原数据表中的第二列）

 ```
 select ename,sal from emp order by 2;
 ```

不建议使用，列发生修改后就不能用了

## 8.综合

关键字顺序不能改变：

``` 
select ... from ... where ... order by ...
```

## 9.数据处理函数

### 1.单行处理函数

数据处理函数又名单行处理函数，特点是一个输入对应一个输出，与多行处理函数（多个输入对应一个输出）相对应。

例如：单行，对一行处理，与其他行无关；多行，比如求和，对每一行的同一列进行处理

### 2.具体的单行处理函数

#### （1）大小写

lower，转换为小写

upper，转换为大写

```
select lower(ename) as name from emp;
select upper(ename) as name from emp;
```

#### （2）截取字符串

substr：substr(被截取的字符串，起始下标，截取的长度)，起始下标从1开始

```
select substr(ename,1,1) as ename from emp;
+-------+
| ename |
+-------+
| S     |
| A     |
| W     |
| J     |
| M     |
| B     |
| C     |
| S     |
| K     |
| T     |
| A     |
| J     |
| F     |
| M     |
+-------+
```



也可以写在查询条件处：

查询ename首字母为A的员工信息

``` 
select * from emp where substr(ename,1,1)='A';
+-------+-------+----------+------+------------+---------+--------+--------+
| EMPNO | ENAME | JOB      | MGR  | HIREDATE   | SAL     | COMM   | DEPTNO |
+-------+-------+----------+------+------------+---------+--------+--------+
|  7499 | ALLEN | SALESMAN | 7698 | 1981-02-20 | 1600.00 | 300.00 |     30 |
|  7876 | ADAMS | CLERK    | 7788 | 1987-05-23 | 1100.00 |   NULL |     20 |
+-------+-------+----------+------+------------+---------+--------+--------+
```



#### （3）拼接字符串

concat:拼接字符串concat(s1,s2)



#### （4）长度

length:获取字段长度



#### （5）去空格

trim:去前后空格

``` 
select ename from emp where ename=trim('   King ');//''扩起来的内容可能是传过来的数据，需要去空格
+-------+
| ename |
+-------+
| KING  |
+-------+
```



#### （6）四舍五入

round:四舍五入 

一个数据库的特点，这一点记住就好：

当查询的不是列名时：

``` 
select 1000 as num from emp;//根据表的记录生成字面量1000
+------+
| num  |
+------+
| 1000 |
| 1000 |
| 1000 |
| 1000 |
| 1000 |
| 1000 |
| 1000 |
| 1000 |
| 1000 |
| 1000 |
| 1000 |
| 1000 |
| 1000 |
| 1000 |
+------+  
```





round(x,d)  ，x指要处理的数，d是指保留几位小数,d可以是负数

``` 
select round(1.2,0) from emp;
+--------------+
| round(1.2,0) |
+--------------+
|            1 |
|            1 |
|            1 |
|            1 |
|            1 |
|            1 |
|            1 |
|            1 |
|            1 |
|            1 |
|            1 |
|            1 |
|            1 |
|            1 |
+--------------+
```



#### （7）随机数

rand():生成随机数

```
select rand()*100 from emp;//100以内的随机数
```



#### （8）处理null

ifnull 空处理函数，在所有数据库中只要有null参与的数学运算，最终结果就是null

例如：计算每个员工的年薪

```
select ename,(sal + comm)*12 as yearsal from emp;
+--------+----------+
| ename  | yearsal  |
+--------+----------+
| SMITH  |     NULL |
| ALLEN  | 22800.00 |
| WARD   | 21000.00 |
| JONES  |     NULL |
| MARTIN | 31800.00 |
| BLAKE  |     NULL |
| CLARK  |     NULL |
| SCOTT  |     NULL |
| KING   |     NULL |
| TURNER | 18000.00 |
| ADAMS  |     NULL |
| JAMES  |     NULL |
| FORD   |     NULL |
| MILLER |     NULL |
+--------+----------+       
```

​                 

comm为null的运算结果也为null

ifnull(数据，若数据为null该数据为这个参数值)

```
select ename,(sal + ifnull(comm,0))*12 as yearsal from emp;
+--------+----------+
| ename  | yearsal  |
+--------+----------+
| SMITH  |  9600.00 |
| ALLEN  | 22800.00 |
| WARD   | 21000.00 |
| JONES  | 35700.00 |
| MARTIN | 31800.00 |
| BLAKE  | 34200.00 |
| CLARK  | 29400.00 |
| SCOTT  | 36000.00 |
| KING   | 60000.00 |
| TURNER | 18000.00 |
| ADAMS  | 13200.00 |
| JAMES  | 11400.00 |
| FORD   | 36000.00 |
| MILLER | 15600.00 |
+--------+----------+
```





#### （9）case

case ... when ... then ... when ... then... else ... end

```
select ename,job,sal, (case job when 'MANAGER' then sal*1.1 when 'SALESMAN' then sal*1.5 else sal end) as newsal
from emp;
+--------+-----------+---------+---------+
| ename  | job       | sal     | newsal  |
+--------+-----------+---------+---------+
| SMITH  | CLERK     |  800.00 |  800.00 |
| ALLEN  | SALESMAN  | 1600.00 | 2400.00 |
| WARD   | SALESMAN  | 1250.00 | 1875.00 |
| JONES  | MANAGER   | 2975.00 | 3272.50 |
| MARTIN | SALESMAN  | 1250.00 | 1875.00 |
| BLAKE  | MANAGER   | 2850.00 | 3135.00 |
| CLARK  | MANAGER   | 2450.00 | 2695.00 |
| SCOTT  | ANALYST   | 3000.00 | 3000.00 |
| KING   | PRESIDENT | 5000.00 | 5000.00 |
| TURNER | SALESMAN  | 1500.00 | 2250.00 |
| ADAMS  | CLERK     | 1100.00 | 1100.00 |
| JAMES  | CLERK     |  950.00 |  950.00 |
| FORD   | ANALYST   | 3000.00 | 3000.00 |
| MILLER | CLERK     | 1300.00 | 1300.00 |
+--------+-----------+---------+---------+


```



## 10.多行处理函数/分组函数

注意：先分组之后才能使用分组函数，未分组则整个表为一组

输入多行最终输出一行结果

count：计数

```
select count(ename) from emp;
```

sum：求和

```
select sum(sal) from emp;
```

avg：求平均值

```
select avg(sal) from emp;
```

max：求最大值

```
select max(sal) from emp;
```

min：求最小值

```
select min(sal) from emp;
```

***

注意：

1.分组函数自动忽略null:

``` 
select sum(comm) from emp;
+-----------+
| sum(comm) |
+-----------+
|   2200.00 |
+-----------+
```





2.分组函数中count(*)和count(具体字段)

count(具体字段)统计该具体字段下不为空的行个数



3.分组函数不能用于where子句

参考关键词的顺序，where在group by之前，组还没分，无法进行分组函数

​	为什么select sum(comm) from emp;可以执行？

​	关键词的运行顺序：

​	from 

​	where 

​	group by  

​	select

​	order by 

​	因为select在group by后执行故可以使用分组函数



## 11.分组查询

先对表进行分组再进行查询

``` 
select ... from ... where ... group by ... order by ...
```

寻找各个部门中的sal最大值

```
select max(sal),deptno from emp group by deptno;
```

+----------+--------+
| max(sal) | deptno |
+----------+--------+
|  5000.00 |     10 |
|  3000.00 |     20 |
|  2850.00 |     30 |
+----------+--------+

结论：如果存在group by子句，注意select后**只能跟group by的参数或者是分组函数**

例如：找出不同部门的不同工作的最高工资：

``` 
select max(sal),deptno,job from emp group by deptno,job;
+----------+--------+-----------+
| max(sal) | deptno | job       |
+----------+--------+-----------+
|  1300.00 |     10 | CLERK     |
|  2450.00 |     10 | MANAGER   |
|  5000.00 |     10 | PRESIDENT |
|  3000.00 |     20 | ANALYST   |
|  1100.00 |     20 | CLERK     |
|  2975.00 |     20 | MANAGER   |
|   950.00 |     30 | CLERK     |
|  2850.00 |     30 | MANAGER   |
|  1600.00 |     30 | SALESMAN  |
+----------+--------+-----------+
```



---

例题：找出部门最大薪资大于3000的部门和最大薪资

```
select deptno,max(sal) from emp group by deptno having max(sal)>3000;
+--------+----------+
| deptno | max(sal) |
+--------+----------+
|     10 |  5000.00 |
+--------+----------+
```



另一种写法：

``` 
select deptno,max(sal) from emp where sal>3000 group by deptno;
```

我们采用第一种写法时，按照部门分组并且计算出每个部门的最高薪资，最后只显示了一个部门，其他的数据作废

这样不如第二种方法，在一开始就筛选出>3000的部门，效率更高

**结论：能用where过滤，尽量用where过滤，保证在前期就扔掉无用数据，避免分组后带入计算**



where没办法解决的情况：

例题：找出部门平均薪资大于3000的部门和最大薪资

数据涉及了整个部门，无法在where中进行过滤



总结：

执行顺序：

1.from 

2.where 

3.group by

4.having

5.select

6.order by 

---

# Mysql笔记,PART2

## 1.distinct去重

```
select distinct job from emp;
+-----------+
| job       |
+-----------+
| CLERK     |
| SALESMAN  |
| MANAGER   |
| ANALYST   |
| PRESIDENT |
+-----------+
```



注意：distinct只能出现在所有字段之前，表示对各个字段都相同的数据去重，因此不能存在在两个字段之间。

对多个字段去重的情况：

```
select distinct job,deptno from emp;
+-----------+--------+
| job       | deptno |
+-----------+--------+
| CLERK     |     20 |
| SALESMAN  |     30 |
| MANAGER   |     20 |
| MANAGER   |     30 |
| MANAGER   |     10 |
| ANALYST   |     20 |
| PRESIDENT |     10 |
| CLERK     |     30 |
| CLERK     |     10 |
+-----------+--------+
```



表示为job为a，deptno为b的数据我只要一个

可以和分组函数一同使用：

```
select count(distinct job,deptno) from emp;
+----------------------------+
| count(distinct job,deptno) |
+----------------------------+
|                          9 |
+----------------------------+
```





## 2.连接查询

### 1.概述

定义：从一张表中查询数据称为单表查询，在多张表进行跨表查询数据称为多表查询。



分类：

​	按照年代分类：

​		sql92：1992年

​		sql99：1999年

​	按照表连接方式分类：

​		1.内连接：

​			等值连接

​			非等值连接

​			自连接

​		2.外连接：

​			左外连接（左连接）

​			右外连接（右连接）

​		3.全连接



### 2.笛卡尔积：

####  1.现象

``` 
 select ename,dname from emp,dept;
 --------+------------+
| ename  | dname      |
+--------+------------+
| SMITH  | ACCOUNTING |
| SMITH  | RESEARCH   |
| SMITH  | SALES      |
| SMITH  | OPERATIONS |
| ALLEN  | ACCOUNTING |
| ALLEN  | RESEARCH   |
| ALLEN  | SALES      |
| ALLEN  | OPERATIONS |

......

共56条数据（14*4），也就是说一个ename对应所有的dname。
```

当我们查询没有任何条件限制时，我们查到的是A表中每个数据和B表中每个数据合在一起形成的大表。



#### 2.如何避免笛卡尔积现象？

加上限制条件即可

```
select ename,dname from emp,dept where emp.deptno = dept.deptno;
+--------+------------+
| ename  | dname      |
+--------+------------+
| CLARK  | ACCOUNTING |
| KING   | ACCOUNTING |
| MILLER | ACCOUNTING |
| SMITH  | RESEARCH   |
| JONES  | RESEARCH   |
| SCOTT  | RESEARCH   |
| ADAMS  | RESEARCH   |
| FORD   | RESEARCH   |
| ALLEN  | SALES      |
| WARD   | SALES      |
| MARTIN | SALES      |
| BLAKE  | SALES      |
| TURNER | SALES      |
| JAMES  | SALES      |
+--------+------------+
```



优化：我们在查询ename时，在emp表查询了一次，在dept表中又查询了一次，为了提高效率，应当**注明字段的表名**

给表名起别名并注明字段的表名：

```
select e.ename,d.dname from emp e,dept d where e.deptno = d.deptno;
```

即使我们加了限制条件，我们连接两个表实际还是进行了14*4次匹配，因此要尽量减少表的连接次数。



### 3.内连接之等值连接

#### 1.sql92和sql99的区别

sql92：//已经不再使用

```
select e.ename,d.dname 
from emp e,dept d 
where e.deptno = d.deptno and ...;//...为我们可能需要的对已连接表的过滤条件
```

缺点是where后跟的是表连接条件和我们后续可能要加的过滤条件。



sql99：

```
select e.ename,d.dname 
from emp e
inner join dept d//inner可以省略
on e.deptno = d.deptno//表连接条件
where ...;//过滤条件
```

sql99表连接的条件是独立的，分离了上述两个条件。



### 4.内连接之非等值连接

条件不是一个等量关系，称为非等值连接

例如：查询员工的薪资等级

``` 
薪资等级：
+-------+-------+-------+
| GRADE | LOSAL | HISAL |
+-------+-------+-------+
|     1 |   700 |  1200 |
|     2 |  1201 |  1400 |
|     3 |  1401 |  2000 |
|     4 |  2001 |  3000 |
|     5 |  3001 |  9999 |
+-------+-------+-------+

select e.ename,e.sal,g.grade
from emp e
inner join salgrade g
on e.sal between g.losal and g.hisal;

+--------+---------+-------+
| ename  | sal     | grade |
+--------+---------+-------+
| SMITH  |  800.00 |     1 |
| ALLEN  | 1600.00 |     3 |
| WARD   | 1250.00 |     2 |
| JONES  | 2975.00 |     4 |
| MARTIN | 1250.00 |     2 |
| BLAKE  | 2850.00 |     4 |
| CLARK  | 2450.00 |     4 |
| SCOTT  | 3000.00 |     4 |
| KING   | 5000.00 |     5 |
| TURNER | 1500.00 |     3 |
| ADAMS  | 1100.00 |     1 |
| JAMES  |  950.00 |     1 |
| FORD   | 3000.00 |     4 |
| MILLER | 1300.00 |     2 |
+--------+---------+-------+
```



### 5.内连接之自连接

自己跟自己连接，相当于把一张表看成两张表



例：查询每个员工和其上级领导，显示员工和上级领导的名字

``` 
select a.ename 'emp',b.ename 'mgr'
from emp a
inner join emp b
on a.mgr = b.empno;

+--------+-------+
| emp    | mgr   |
+--------+-------+
| SMITH  | FORD  |
| ALLEN  | BLAKE |
| WARD   | BLAKE |
| JONES  | KING  |
| MARTIN | BLAKE |
| BLAKE  | KING  |
| CLARK  | KING  |
| SCOTT  | JONES |
| TURNER | BLAKE |
| ADAMS  | SCOTT |
| JAMES  | BLAKE |
| FORD   | JONES |
| MILLER | CLARK |
+--------+-------+
13 rows in set (0.00 sec)//king没有领导
```



### 6.外连接

案例：查出所有部门的所有员工的名字和部门名字

```
部门表，其中有一个部门40没有任何员工
+--------+------------+----------+
| DEPTNO | DNAME      | LOC      |
+--------+------------+----------+
|     10 | ACCOUNTING | NEW YORK |
|     20 | RESEARCH   | DALLAS   |
|     30 | SALES      | CHICAGO  |
|     40 | OPERATIONS | BOSTON   |
+--------+------------+----------+

+-------+--------+-----------+------+------------+---------+---------+--------+
| EMPNO | ENAME  | JOB       | MGR  | HIREDATE   | SAL     | COMM    | DEPTNO |
+-------+--------+-----------+------+------------+---------+---------+--------+
|  7369 | SMITH  | CLERK     | 7902 | 1980-12-17 |  800.00 |    NULL |     20 |
|  7499 | ALLEN  | SALESMAN  | 7698 | 1981-02-20 | 1600.00 |  300.00 |     30 |
|  7521 | WARD   | SALESMAN  | 7698 | 1981-02-22 | 1250.00 |  500.00 |     30 |
|  7566 | JONES  | MANAGER   | 7839 | 1981-04-02 | 2975.00 |    NULL |     20 |
|  7654 | MARTIN | SALESMAN  | 7698 | 1981-09-28 | 1250.00 | 1400.00 |     30 |
|  7698 | BLAKE  | MANAGER   | 7839 | 1981-05-01 | 2850.00 |    NULL |     30 |
|  7782 | CLARK  | MANAGER   | 7839 | 1981-06-09 | 2450.00 |    NULL |     10 |
|  7788 | SCOTT  | ANALYST   | 7566 | 1987-04-19 | 3000.00 |    NULL |     20 |
|  7839 | KING   | PRESIDENT | NULL | 1981-11-17 | 5000.00 |    NULL |     10 |
|  7844 | TURNER | SALESMAN  | 7698 | 1981-09-08 | 1500.00 |    0.00 |     30 |
|  7876 | ADAMS  | CLERK     | 7788 | 1987-05-23 | 1100.00 |    NULL |     20 |
|  7900 | JAMES  | CLERK     | 7698 | 1981-12-03 |  950.00 |    NULL |     30 |
|  7902 | FORD   | ANALYST   | 7566 | 1981-12-03 | 3000.00 |    NULL |     20 |
|  7934 | MILLER | CLERK     | 7782 | 1982-01-23 | 1300.00 |    NULL |     10 |
+-------+--------+-----------+------+------------+---------+---------+--------+
内连接的做法
select e.ename,d.dname
from emp e
inner join dept d
on e.deptno = d.deptno;
结果：
+--------+------------+
| ename  | dname      |
+--------+------------+
| CLARK  | ACCOUNTING |
| KING   | ACCOUNTING |
| MILLER | ACCOUNTING |
| SMITH  | RESEARCH   |
| JONES  | RESEARCH   |
| SCOTT  | RESEARCH   |
| ADAMS  | RESEARCH   |
| FORD   | RESEARCH   |
| ALLEN  | SALES      |
| WARD   | SALES      |
| MARTIN | SALES      |
| BLAKE  | SALES      |
| TURNER | SALES      |
| JAMES  | SALES      |
+--------+------------+
14 rows in set (0.00 sec)
我们发现由于BOSTON没有任何员工，导致连部门名也没有查出
```

解决方法：采用外连接:

```
select e.ename,d.dname
from emp e
right join dept d
on e.deptno = d.deptno;

+--------+------------+
| ename  | dname      |
+--------+------------+
| CLARK  | ACCOUNTING |
| KING   | ACCOUNTING |
| MILLER | ACCOUNTING |
| SMITH  | RESEARCH   |
| JONES  | RESEARCH   |
| SCOTT  | RESEARCH   |
| ADAMS  | RESEARCH   |
| FORD   | RESEARCH   |
| ALLEN  | SALES      |
| WARD   | SALES      |
| MARTIN | SALES      |
| BLAKE  | SALES      |
| TURNER | SALES      |
| JAMES  | SALES      |
| NULL   | OPERATIONS |
+--------+------------+
15 rows in set (0.00 sec)
这里采用了右外连接，右边的表dept为主表，emp为副表，意思是我们需要把主表中的数据查出来，然后查出副表中符合条件的数据
```

外连接和内连接的区别在于，内连接没有主表副表关系之分，两张表是平等的关系；而外连接有主次关系。



任何右外连接都可以换为左外连接，调换一下两张表的位置：

```
select e.ename,d.dname
from dept d
left outer join emp e
on e.deptno = d.deptno;
其中outer可以省略

+--------+------------+
| ename  | dname      |
+--------+------------+
| CLARK  | ACCOUNTING |
| KING   | ACCOUNTING |
| MILLER | ACCOUNTING |
| SMITH  | RESEARCH   |
| JONES  | RESEARCH   |
| SCOTT  | RESEARCH   |
| ADAMS  | RESEARCH   |
| FORD   | RESEARCH   |
| ALLEN  | SALES      |
| WARD   | SALES      |
| MARTIN | SALES      |
| BLAKE  | SALES      |
| TURNER | SALES      |
| JAMES  | SALES      |
| NULL   | OPERATIONS |
+--------+------------+
15 rows in set (0.00 sec)
```

例题：查询出员工的上级领导，显示所有员工以及其领导的名字

```
select a.ename,b.ename 
from emp a
left outer join emp b
on a.mgr = b.empno;
+--------+-------+
| ename  | ename |
+--------+-------+
| SMITH  | FORD  |
| ALLEN  | BLAKE |
| WARD   | BLAKE |
| JONES  | KING  |
| MARTIN | BLAKE |
| BLAKE  | KING  |
| CLARK  | KING  |
| SCOTT  | JONES |
| KING   | NULL  |
| TURNER | BLAKE |
| ADAMS  | SCOTT |
| JAMES  | BLAKE |
| FORD   | JONES |
| MILLER | CLARK |
+--------+-------+
14 rows in set (0.00 sec)
这次查出了KING
```

### 7.多张表的连接

```
select ...
from a
join b
on ab连接条件
join c
on ac连接条件
left join d
on ad连接条件
...
在一次查询中内外连接都可以采用
```

 例题：找出每个员工的部门名称以及工资等级，显示员工名，部门名，薪资，薪资等级

```
select e.ename,d.dname,e.sal,g.grade
from emp e
join dept d
on e.deptno = d.deptno
join salgrade g
on e.sal between g.losal and g.hisal;

+--------+------------+---------+-------+
| ename  | dname      | sal     | grade |
+--------+------------+---------+-------+
| SMITH  | RESEARCH   |  800.00 |     1 |
| ALLEN  | SALES      | 1600.00 |     3 |
| WARD   | SALES      | 1250.00 |     2 |
| JONES  | RESEARCH   | 2975.00 |     4 |
| MARTIN | SALES      | 1250.00 |     2 |
| BLAKE  | SALES      | 2850.00 |     4 |
| CLARK  | ACCOUNTING | 2450.00 |     4 |
| SCOTT  | RESEARCH   | 3000.00 |     4 |
| KING   | ACCOUNTING | 5000.00 |     5 |
| TURNER | SALES      | 1500.00 |     3 |
| ADAMS  | RESEARCH   | 1100.00 |     1 |
| JAMES  | SALES      |  950.00 |     1 |
| FORD   | RESEARCH   | 3000.00 |     4 |
| MILLER | ACCOUNTING | 1300.00 |     2 |
+--------+------------+---------+-------+
14 rows in set (0.00 sec)
```

例题：找出每个员工的部门名称以及工资等级，显示员工名，部门名，薪资，薪资等级，**上级领导**

```
select e.ename,d.dname,e.sal,g.grade
from emp e
join dept d
on e.deptno = d.deptno
join salgrade g
on e.sal between g.losal and g.hisal
left join emp m
on e.mgr = m.empno;

+--------+------------+---------+-------+
| ename  | dname      | sal     | grade |
+--------+------------+---------+-------+
| SMITH  | RESEARCH   |  800.00 |     1 |
| ALLEN  | SALES      | 1600.00 |     3 |
| WARD   | SALES      | 1250.00 |     2 |
| JONES  | RESEARCH   | 2975.00 |     4 |
| MARTIN | SALES      | 1250.00 |     2 |
| BLAKE  | SALES      | 2850.00 |     4 |
| CLARK  | ACCOUNTING | 2450.00 |     4 |
| SCOTT  | RESEARCH   | 3000.00 |     4 |
| KING   | ACCOUNTING | 5000.00 |     5 |
| TURNER | SALES      | 1500.00 |     3 |
| ADAMS  | RESEARCH   | 1100.00 |     1 |
| JAMES  | SALES      |  950.00 |     1 |
| FORD   | RESEARCH   | 3000.00 |     4 |
| MILLER | ACCOUNTING | 1300.00 |     2 |
+--------+------------+---------+-------+
14 rows in set (0.00 sec)
```

## 3.子查询

### 1.概述

（1）定义：select语句中嵌套select语句，被嵌套的select语句被称为子查询。

（2）子查询出现的位置：

```
select 
	..(select)
from 
	..(select)
where 
	..(select)
```

### 2.where子句中的子查询

案例：找出比最低工资高的员工名和薪资

```
第一步，查询最低工资
select min(sal) from emp;
+----------+
| min(sal) |
+----------+
|   800.00 |
+----------+
1 row in set (0.00 sec)

第二步，查询比800高的员工名和薪资
select ename,sal from emp where sal>800;

第三步，合并
select ename,sal from emp where sal>( select min(sal) from emp );
+--------+---------+
| ename  | sal     |
+--------+---------+
| ALLEN  | 1600.00 |
| WARD   | 1250.00 |
| JONES  | 2975.00 |
| MARTIN | 1250.00 |
| BLAKE  | 2850.00 |
| CLARK  | 2450.00 |
| SCOTT  | 3000.00 |
| KING   | 5000.00 |
| TURNER | 1500.00 |
| ADAMS  | 1100.00 |
| JAMES  |  950.00 |
| FORD   | 3000.00 |
| MILLER | 1300.00 |
+--------+---------+
```

### 3.from子句中的子查询

from后面的子查询，可以将子查询的查询结果看成一张临时表

例题：找出每个岗位的平均薪资的薪资等级

```
1.按job查询薪资
select job,avg(sal) from emp group by job;

+-----------+-------------+
| job       | avg(sal)    |
+-----------+-------------+
| ANALYST   | 3000.000000 |
| CLERK     | 1037.500000 |
| MANAGER   | 2758.333333 |
| PRESIDENT | 5000.000000 |
| SALESMAN  | 1400.000000 |
+-----------+-------------+

2.按平均薪资avg（sal）查询薪资等级
select a.job,a.avg(sal),g.grade 
from (select job,avg(sal) from emp group by job) a
inner join salgrade g
on a.avg(sal) between g.losal and g.hisal;

ERROR 1630 (42000): FUNCTION a.avg does not exist. Check the 'Function Name Parsing and Resolution' section in the Reference Manual
这里报错的原因是，在on a.avg(sal)，程序认为avg是函数，而这里本该是我们a表中的字段
解决方法：起别名

3.按平均薪资avgsal查询薪资等级
select a.job,a.avgsal,g.grade 
from (select job,avg(sal) as avgsal from emp group by job) a
inner join salgrade g
on a.avgsal between g.losal and g.hisal;

+-----------+-------------+-------+
| job       | avgsal      | grade |
+-----------+-------------+-------+
| CLERK     | 1037.500000 |     1 |
| SALESMAN  | 1400.000000 |     2 |
| ANALYST   | 3000.000000 |     4 |
| MANAGER   | 2758.333333 |     4 |
| PRESIDENT | 5000.000000 |     5 |
+-----------+-------------+-------+
5 rows in set (0.00 sec)
```

### 3.select子句后的子查询

案例：找出每个员工的部门名称，要求显示员工名和部门名

````
select ename,deptno,(select dname from dept d where e.deptno = d.deptno)
from emp e;
+--------+--------+------------------------------------------------------+
| ename  | deptno | (select dname from dept d where e.deptno = d.deptno) |
+--------+--------+------------------------------------------------------+
| SMITH  |     20 | RESEARCH                                             |
| ALLEN  |     30 | SALES                                                |
| WARD   |     30 | SALES                                                |
| JONES  |     20 | RESEARCH                                             |
| MARTIN |     30 | SALES                                                |
| BLAKE  |     30 | SALES                                                |
| CLARK  |     10 | ACCOUNTING                                           |
| SCOTT  |     20 | RESEARCH                                             |
| KING   |     10 | ACCOUNTING                                           |
| TURNER |     30 | SALES                                                |
| ADAMS  |     20 | RESEARCH                                             |
| JAMES  |     30 | SALES                                                |
| FORD   |     20 | RESEARCH                                             |
| MILLER |     10 | ACCOUNTING                                           |
+--------+--------+------------------------------------------------------+
注意，在select子句中的子查询可以拿到每一轮外层查询的数据(本例中为e.*)，然后再进行查询，显示为个行数据中的一列，所以每次只能查到一行数据
````

**select子句中的子查询每一轮查出的数据必须只有一行**

## 4.union合并查询结果集

查询经理和销售员的职员名字和职位

```
select ename,job from emp where emp.job='MANAGER'
union
select ename,job from emp where emp.job='SALESMAN';
+--------+----------+
| ename  | job      |
+--------+----------+
| JONES  | MANAGER  |
| BLAKE  | MANAGER  |
| CLARK  | MANAGER  |
| ALLEN  | SALESMAN |
| WARD   | SALESMAN |
| MARTIN | SALESMAN |
| TURNER | SALESMAN |
+--------+----------+
7 rows in set (0.00 sec)
```

union的作用，减少表的连接以减少计算量

比如abc三个表连接（数据量都是10），连接后就是1000条数据

如果采用先连接两个表然后union，数据量为10*10+100=200，把乘法变为了加法。

其他注意事项：

1.两个结果集合并时，列数一定要相同

```
select ename,job from emp where emp.job='MANAGER'
union
select ename from emp where emp.job='SALESMAN';
报错
```

2.两个结果集对应的列的数据类型也要相同（mysql不报错，oracle报错）

```
select ename,job from emp where emp.job='MANAGER'
union
select ename,sal from emp where emp.job='SALESMAN';
报错
```

## 5.limit

### 1.概述

将查询结果集的一部分取出，而不是显示所有数据。通常使用在分页查询中，例如百度一页显示10条记录。

### 2.使用方法

查出工资降序排列的前五条数据

```
select ename,sal
from emp
order by sal desc
limit 0,5;
0：从第0条数据开始拿，5：拿取5条
+-------+---------+
| ename | sal     |
+-------+---------+
| KING  | 5000.00 |
| SCOTT | 3000.00 |
| FORD  | 3000.00 |
| JONES | 2975.00 |
| BLAKE | 2850.00 |
+-------+---------+

select ename,sal
from emp
order by sal desc
limit 5;
可以省略0，5代表取出的长度
```

注意：**limit在order by后执行**

## 6.DQL总结

```
select ...
from ...
where ...
group by ...
having ...
order by ...
limit ...
执行顺序：
from,where,group by,having,select,order by,limit
```

## 7.表的创建

### 1.语法格式

```
create table 表名(字段名1 数据类型,字段名2 数据类型,...);

create table 表名(
字段名1 数据类型,
字段名2 数据类型,
...
);
```

表名以t_开始

或者以tbl_开始，表名和字段名都属于标识符

### 2.mysql中的数据类型

```
这里的数字，如varchar(10)中的10都是指字符长度而不是字节，中文“中”也代表1个长度

varchar:最长255
	可变长度的字符串，根据传过来数据的实际长度动态分配空间	
	优点：节省空间；缺点：执行效率低
char:最长255
	定长字符串，
	不管实际长度多少，分配固定的空间存储数据
	优点：不动态分配空间，执行效率高；缺点：空间浪费
	选择方法：性别这种定长的选择char
int:最长11
	整型数据，java中的int
bigint:
	长整型数据，java中的long
float:
	单精度浮点型数据
double:
	双精度浮点型数据
data:
	短日期类型
datatime:
	长日期；类型
clob:
	字符大对象，最多可以存储4G的字符串，存储一个文章等，超过255个字符的用clob
blob:
	二进制大对象
	专门存储图片，声音，视频等流媒体数据
	向blob中插入数据必须要用io流
```

### 3.创建一个表

```
create table t_student(
	no int,
	name varchar(32),
	gender char(1) default 'm',
	age int(3),
	email varchar(255)
);

gender char(1) default 'm'可以在建表时指定默认值，不指定时默认为null
注意是()而不是{}
age int(3),//这里是建议长度为3,最大为999

删除表：
drop table t_student;
不成功会报错

drop table if exists t_student; 
不存在该表也不会报错

复制一张表
create table emp2 as select * from emp;
```

### 4.插入数据

格式：

```
insert into t_student(字段名1，字段名2，...) values(值1,值2,...);

字段名顺序可以改，只要跟值对应即可，只插入部分值也可以,不写字段默认是插入所有字段
insert into t_student(no,name,gender,age,email) values(1,'tom','m',20,'tom@123.com');
insert into t_student(no) values(2);
insert into t_student values(3,'jack','m',20,'jack@123.com');
+------+------+--------+------+--------------+
| no   | name | gender | age  | email        |
+------+------+--------+------+--------------+
|    1 | tom  | m      |   20 | tom@123.com  |
|    2 | NULL | NULL   | NULL | NULL         |
|    3 | jack | m      |   20 | jack@123.com |
+------+------+--------+------+--------------+

将一张表的内容插入另一张表中：
create table dept_bak select * from dept;
insert into dept_bak select * from dept;
+--------+------------+----------+
| DEPTNO | DNAME      | LOC      |
+--------+------------+----------+
|     10 | ACCOUNTING | NEW YORK |
|     20 | RESEARCH   | DALLAS   |
|     30 | SALES      | CHICAGO  |
|     40 | OPERATIONS | BOSTON   |
|     10 | ACCOUNTING | NEW YORK |
|     20 | RESEARCH   | DALLAS   |
|     30 | SALES      | CHICAGO  |
|     40 | OPERATIONS | BOSTON   |
+--------+------------+----------+
```

### 5.插入日期

```
str_to_date:字符串varchar转换为date类型
date_format:将date类型转换为具有一定格式的varchar字符串类型
```

注意：数据库中有一个命名规范，所有标识符都是小写，单词单词之间用下划线连接



先建立一个新表：

```
create table t_user(
	id int,
	name varchar(32),
	birth date
);
```

mysql中的日期格式:

%Y年,注意要大写

%m月

%d日

%h时

%i分

%s秒

在新表中插入数据：

```
insert into t_user values(1,'Marisa',str_to_date('01-10-2005','%d-%m-%Y'));
+------+--------+------------+
| id   | name   | birth      |
+------+--------+------------+
|    1 | Marisa | 2005-10-01 |
+------+--------+------------+
一次插入多条记录：
insert into t_user(id,name,birth) values
(1,'Marisa','2208-08-06'),
(2,'Marisa','2005-04-05');
```

如果采用2001-10-01这种格式可以不用使用str_to_date，mysql自动转换

```
insert into t_user values(1,'Marisa','2005-10-01');
+------+--------+------------+
| id   | name   | birth      |
+------+--------+------------+
|    1 | Marisa | 2005-10-01 |
|    1 | Marisa | 2005-10-01 |
+------+--------+------------+
```



date_format:将date转换为格式化的字符串

```
select id,name,date_format(birth,'%Y/%m/%d') as birth from t_user;
+------+--------+------------+
| id   | name   | birth      |
+------+--------+------------+
|    1 | Marisa | 2005/10/01 |
|    1 | Marisa | 2005/10/01 |
+------+--------+------------+
```

当然，数据库中存储的是date类型，显示到屏幕的数据也是自动经过mysql自动转换的格式化字符串

### 6.date和datetime

date为短日期，包括年月日

datetime为长日期，包括年月日时分秒

msyql默认格式：

%Y-%m-%d  

%Y%m%d  %h:%i:%s		

now():表示当前长日期



### 7.修改update(DML)

```
update t_user set name = 'jack', birth='2000-10-18' where id = 1;
+------+------+------------+
| id   | name | birth      |
+------+------+------------+
|    1 | jack | 2000-10-18 |
|    1 | jack | 2000-10-18 |
+------+------+------------+
不加where会导致全表修改
update t_user set name = 'abc';
+------+------+------------+
| id   | name | birth      |
+------+------+------------+
|    1 | abc  | 2000-10-18 |
|    1 | abc  | 2000-10-18 |
+------+------+------------+
```

### 8.删除delete

```
delete from t_user where id = 1;
delete from t_user;//不写条件则全部删除

delete删除数据比较慢，表中的数据被删除了，数据在硬盘的存储空间没有释放，可以回滚
start transaction;
delete from dept_bak;
rollback;
数据没有影响

truncate删除，效率高，物理删除，不能回滚
truncate table dept_bak;
```

# Mysql笔记PART3

## 1.对表结构的增删改

一般不进行修改，因为改表结构就要修改所有代码，使用alter修改

## 2.约束constraint

### 1.概述

在创建表时给表的字段加上一些约束，以保证表中数据的完整性和有效性

约束分类：

​	非空约束：not null

​	唯一性约束：unique

​	主键约束：primary key

​	外键约束：foreign key

​	检查约束： check（mysql不支持，oracle支持）

### 1.非空约束

非空约束约束的字段不能为空

```
drop table if exists t_vip;
create table t_vip(
	id int,
	name varchar(255) not null
);
insert into t_vip(id,name) values(1,'zhangsan');
insert into t_vip(id,name) values(2,'lisi');

insert into t_vip(id) values(3);
ERROR 1364 (HY000): Field 'name' doesn't have a default value
```

顺带提，sql脚本要用cmd导入而不要复制粘贴，数据过多时记事本根本打不开,导入命令：

```
source 路径;
```

### 2.唯一性约束

不能重复但是可以为null

```
drop table if exists t_vip;
create table t_vip(
	id int unique, //列级约束
	name varchar(255)
);
insert into t_vip(id,name) values(1,'zhangsan');
不能重复：
insert into t_vip(id,name) values(1,'lisi');
ERROR 1062 (23000): Duplicate entry '1' for key 'id'
可以为null：
insert into t_vip(name) values('lisi');
insert into t_vip(name) values('lisi');
+------+----------+
| id   | name     |
+------+----------+
|    1 | zhangsan |
| NULL | lisi     |
| NULL | lisi     |
+------+----------+
```

id int unique, //列级约束



新需求，创建一张表，包含id name email字段，name和email**联合**起来唯一:

```
drop table if exists t_vip;
create table t_vip(
	id int,
	name varchar(255),
	email varchar(255),
	unique(name,email) 
);
insert into t_vip(id,name,email) values(1,'zhangsan','zs@123.com');
insert into t_vip(id,name,email) values(1,'zhangsan','zs@155.com');
+------+----------+------------+
| id   | name     | email      |
+------+----------+------------+
|    1 | zhangsan | zs@123.com |
|    1 | zhangsan | zs@155.com |
+------+----------+------------+
```

unique(name,email) ：表级约束，需要给多个字段联合起来添加一个约束时使用，**not null没有表级约束**

在mysql中，如果1个字段被not null和unique同时约束，该字段称为主键字段（oracle不同）

### 3.主键约束

1.主键约束的相关术语：

​	主键约束：约束名

​	主键字段：加了主键约束的字段

​	主键值：主键字段的每一个值

2.主键的特征： 非空不能重复

添加主键约束

```
drop table if exists t_vip;
create table t_vip(
	id int primary key,
	name varchar(255),
	email varchar(255)
);
insert into t_vip(id,name,email) values(1,'zhangsan','zs@123.com');

//错误：不能重复
insert into t_vip(id,name,email) values(1,'zhangsan','zs@155.com');
ERROR 1062 (23000): Duplicate entry '1' for key 'PRIMARY'
//不能为空
insert into t_vip(name,email) values('zhangsan','zs@155.com');
ERROR 1364 (HY000): Field 'id' doesn't have a default value
```

主键约束可以使用表级约束。

一个字段作为主键称为单一主键，多个字段作为主键称为复合主键，**实际开发中不建议使用复合主键**，因为单一主键可以达到目的

```
drop table if exists t_vip;
create table t_vip(
	id int,
	name varchar(255),
	email varchar(255),
	primary key(name,email)
);
insert into t_vip(id,name,email) values(1,'zhangsan','zs@123.com');
insert into t_vip(id,name,email) values(1,'zhangsan','zs@155.com');
+------+----------+------------+
| id   | name     | email      |
+------+----------+------------+
|    1 | zhangsan | zs@123.com |
|    1 | zhangsan | zs@155.com |
+------+----------+------------+
```

主键可以添加两个吗？不能

```
drop table if exists t_vip;
create table t_vip(
	id int,
	name varchar(255) primary key,
	email varchar(255) primary key
);
ERROR 1068 (42000): Multiple primary key defined
```

### 4.主键的分类

自然主键：一个自然数，与业务无关

业务主键：与业务紧密关联的主键，比如拿银行卡号作为主键值

选择：尽量选择自然主键，防止业务更改时对主键的操作

尽量不要使用varchar作为主键，主键一般选择定长的数字。



自增的主键:

 ```
 drop table if exists t_vip;
 create table t_vip(
 	id int primary key auto_increment,
 	name varchar(255),
 	email varchar(255) 
 );
 insert into t_vip(name,email) values('zhangsan','zs@123.com');
 insert into t_vip(name,email) values('zhangsan','zs@123.com');
 insert into t_vip(name,email) values('zhangsan','zs@123.com');
 +----+----------+------------+
 | id | name     | email      |
 +----+----------+------------+
 |  1 | zhangsan | zs@123.com |
 |  2 | zhangsan | zs@123.com |
 |  3 | zhangsan | zs@123.com |
 +----+----------+------------+
 ```

### 5.外键约束

相关术语：

​	外键约束：foreign key

​	外键字段：加上外键约束的字段

​	外键值：外键字段中的每个值



设计数据库表描述班级和学生的信息？

方法1：设计一张大表：缺点是存在大量重复数据，比如班级的名字很长“xx市xx区第x中学x年级x班”，每个学生都必须有班级名，冗余很大。因此应当采用班级编号连接两张表的方式创建。

方法2：设计一个班级表和一个学生表，学生表中的班级号来自班级表，使用外键约束

注意：**使用外键约束意味着有父子关系**，本例中，班级表为父表，应当注意删除，创建的顺序

```
drop table if exists t_student;
drop table if exists t_school;

create table t_school(
	sid int primary key,
	name varchar(32)
);
create table t_student(
	id int primary key auto_increment,
	name varchar(32),
	sid int,
	foreign key(sid) references t_school(sid)
);

insert into t_school values(100,'no100school');
insert into t_school values(101,'no101school');

insert into t_student (name,sid) values('A',100);
insert into t_student (name,sid) values('B',100);
insert into t_student (name,sid) values('C',100);
insert into t_student (name,sid) values('D',101);
insert into t_student (name,sid) values('E',101);

school:
+-----+-------------+
| sid | name        |
+-----+-------------+
| 100 | no100school |
| 101 | no101school |
+-----+-------------+
student:
+----+------+------+
| id | name | sid  |
+----+------+------+
|  1 | A    |  100 |
|  2 | B    |  100 |
|  3 | C    |  100 |
|  4 | D    |  101 |
|  5 | E    |  101 |
+----+------+------+
//报错,没有102编号的学校
insert into t_student (name,sid) values('F',102);
ERROR 1452 (23000): Cannot add or update a child row: a foreign key constraint fails (`almond`.`t_student`, CONSTRAINT `t_student_ibfk_1` FOREIGN KEY (`sid`) REFERENCES `t_school` (`sid`))
```

父表中被引用的外键**必须具有唯一性**，不一定是主键。不过不是主键会报错error150，需要创建索引？



## 3.存储引擎

定义：一个表存储/组织数据的方式，不同的存储引擎有不同存储数据的方式

如何指定存储引擎？

```
查看创建表时的语句：
show create table t_student;
得到：
| t_student | CREATE TABLE `t_student` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(32) DEFAULT NULL,
  `sid` int(11) DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `sid` (`sid`),
  CONSTRAINT `t_student_ibfk_1` FOREIGN KEY (`sid`) REFERENCES `t_school` (`sid`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8 |

最后一行ENGINE=InnoDB，说明建表时可指定存储引擎

建表时指定存储引擎以及字符编码方式
create table t_product(
	id int primary key,
	name varchar(32)
)engine=InnoDB default charset=gbk;
查看建表方式，发现字符集为gbk
| t_product | CREATE TABLE `t_product` (
  `id` int(11) NOT NULL,
  `name` varchar(32) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk |

```

查看mysql支持的引擎

```
show engines \G
```

三种引擎（了解为主）：



MyISAM存储引擎？
	它管理的表具有以下特征：
		使用三个文件表示每个表：
			格式文件 — 存储表结构的定义（mytable.frm）
			数据文件 — 存储表行的内容（mytable.MYD）
			索引文件 — 存储表上索引（mytable.MYI）：索引是一本书的目录，缩小扫描范围，提高查询效率的一种机制。
		可被转换为压缩、只读表来节省空间

		提示一下：
			对于一张表来说，只要是主键，
			或者加有unique约束的字段上会自动创建索引。
	
		MyISAM存储引擎特点：
			可被转换为压缩、只读表来节省空间
			这是这种存储引擎的优势！！！！
		
		MyISAM不支持事务机制，安全性低。



InnoDB存储引擎？
	这是mysql默认的存储引擎，同时也是一个重量级的存储引擎。
	InnoDB支持事务，支持数据库崩溃后自动恢复机制。
	InnoDB存储引擎最主要的特点是：非常安全。

	它管理的表具有下列主要特征：
		– 每个 InnoDB 表在数据库目录中以.frm 格式文件表示
		– InnoDB 表空间 tablespace 被用于存储表的内容（表空间是一个逻辑名称。表空间存储数据+索引。）
	
		– 提供一组用来记录事务性活动的日志文件
		– 用 COMMIT(提交)、SAVEPOINT 及ROLLBACK(回滚)支持事务处理
		– 提供全 ACID 兼容
		– 在 MySQL 服务器崩溃后提供自动恢复
		– 多版本（MVCC）和行级锁定
		– 支持外键及引用的完整性，包括级联删除和更新
	
	InnoDB最大的特点就是支持事务：
		以保证数据的安全。效率不是很高，并且也不能压缩，不能转换为只读，
		不能很好的节省存储空间。



MEMORY存储引擎？
	使用 MEMORY 存储引擎的表，其数据存储在内存中，且行的长度固定，
	这两个特点使得 MEMORY 存储引擎非常快。

	MEMORY 存储引擎管理的表具有下列特征：
		– 在数据库目录内，每个表均以.frm 格式的文件表示。
		– 表数据及索引被存储在内存中。（目的就是快，查询快！）
		– 表级锁机制。
		– 不能包含 TEXT 或 BLOB 字段。
	
	MEMORY 存储引擎以前被称为HEAP 引擎。
	
	MEMORY引擎优点：查询效率是最高的。不需要和硬盘交互。
	MEMORY引擎缺点：不安全，关机之后数据消失。因为数据和索引都是在内存当中。



## 4.事务

### 1.概述

定义：一个事务是一个完成的业务逻辑。例如，从a向b转账100：a-=100 b+=100 这就是一个完成的业务逻辑，是一个最小的工作单元，不可再分，要么同时成功或者同时失败。

只有insert，update，delete跟事务有关，其他跟事务无关。当操作设计到数据的增删改，就要考虑安全问题。

### 2.事务的具体实现

InnoDB引擎：提供了一组用于记录事务性活动的日志文件

在执行事务时，每一条DML语句都会被记录在日志文件中。

当执行事务时，可以提交事务或者回滚事务：

​	提交事务：清空日志，将数据全部彻底持久化到数据库表中

​	回滚事务：将之前的所有DML操作全部撤销，清空日志

### 3.提交/回滚事务

开启事务，关闭mysql自动提交机制：

```
start transaction;
```

提交事务：

```
commit;
```

回滚事务：

```
rollback;
```

mysql自动提交机制：mysql默认情况下，在每执行一次DML语句都进行一次提交。

回滚只能回到上一次提交点。

演示事务：

```
select * from dept_bak;
+--------+------------+----------+
| DEPTNO | DNAME      | LOC      |
+--------+------------+----------+
|     10 | ACCOUNTING | NEW YORK |
|     20 | RESEARCH   | DALLAS   |
|     30 | SALES      | CHICAGO  |
|     40 | OPERATIONS | BOSTON   |
|     10 | ACCOUNTING | NEW YORK |
|     20 | RESEARCH   | DALLAS   |
|     30 | SALES      | CHICAGO  |
|     40 | OPERATIONS | BOSTON   |
+--------+------------+----------+
delete from dept_bak;
 
select * from dept_bak;
Empty set (0.00 sec)//删除成功

rollback;
select * from dept_bak;
+--------+------------+----------+
| DEPTNO | DNAME      | LOC      |
+--------+------------+----------+
|     10 | ACCOUNTING | NEW YORK |
|     20 | RESEARCH   | DALLAS   |
|     30 | SALES      | CHICAGO  |
|     40 | OPERATIONS | BOSTON   |
|     10 | ACCOUNTING | NEW YORK |
|     20 | RESEARCH   | DALLAS   |
|     30 | SALES      | CHICAGO  |
|     40 | OPERATIONS | BOSTON   |
+--------+------------+----------+
```

### 4.事务的四个特性

A：原子性，事务是最小工作单元，不可分

C：一致性，一个事务过程中的操作要么同时成功，要么同时失败

I：隔离性，两个事务之间有一定的隔离。如：A事务在操作一张表时，另一个事务B不能操作该表

D：持久性，事务最终结果的一个保障，事务提交，就相当于将没有保存到硬盘的数据保存到硬盘上。



### 5.事务的隔离性

隔离有不同的隔离级别：

#### 1）读未提交: read uncommitted

事务A可以读到事务B未提交的数据

产生的问题：脏读（dirty read），读到了脏数据。

几乎没有数据库使用这一级别

#### 2）读已提交: read comitted

事务A可以读到事务B已经提交的数据。

解决的问题：不会进行脏读

存在的问题：不可重复读取，即重复读取到的数据不一致，例如：A事务反复读取数据，B事务在操作数据，导致A每一次读到的数据不相同。

这种隔离级别可以保证我们读到的数据一定是真实的数据

oracle的默认隔离级别是读已提交

#### 3）可重复读: repeatable read

事务A开启后，不论A持续多久，A事务每一次读取到的数据都是一致的，即使B已经修改了数据并成功提交了。

解决的问题：不可重复读

出现的问题：幻影读，读到的数据不真实

幻读：可重复读保证了我们多次读取时显示的数据是不改变的，**但是查询的数据并不是真实的数据**，真实的数据可能已经改变，我们读到的是幻象，我们查到了这个数据，不代表它真实存在，我们查不到这个数据，不代表他真实不存在

对幻读的测试：可重复读状态下

```
事务1：
select * from user_table;
+------+----------+---------+
| user | password | balance |
+------+----------+---------+
| AA   | 123456   |    1000 |
| BB   | 654321   |    1000 |
| CC   | abcd     |    2000 |
| DD   | abcder   |    3000 |
| EE   | 123      |    NULL |
+------+----------+---------+

事务2：
insert into user_table(user,password) values('xx',123);
commit;
事务1：
select * from user_table;
+------+----------+---------+
| user | password | balance |
+------+----------+---------+
| AA   | 123456   |    1000 |
| BB   | 654321   |    1000 |
| CC   | abcd     |    2000 |
| DD   | abcder   |    3000 |
| EE   | 123      |    NULL |
+------+----------+---------+
delete from user_table where user='xx';
删除了查询结果中不存在的数据
```

加锁查询和增删都是对于真实数据的操作，而普通查询针对该事务的查询结果集而言，**查询结果集和真实数据的不同是幻读的根本原因**



mysql默认的隔离级别是可重复读

使用案例：银行想要查询总账，1点开始4点完成，在中间可能有人存款取款，这时采取可重复读隔离级别。相当于把一点那一时刻的记录保存下来记账，其他存款取款业务对记账事务没有影响。

#### 4）序列化/串行化:	serializable

最高隔离级别，效率最低，解决所有的问题，不能并发。相当于synchronize线程同步。



查看隔离级别

```
select @@tx_isolation;
默认隔离级别：
+-----------------+
| @@tx_isolation  |
+-----------------+
| REPEATABLE-READ |
+-----------------+
```



验证：

read uncommitted:



A设置全局隔离级别

```
set global transaction isolation level read uncommitted;
```

A注意要退出重新登录一下mysql，进入后查看隔离级别：

```
select @@tx_isolation;
+----------------+
| @@tx_isolation |
+----------------+
| READ-COMMITTED |
+----------------+
```

A切换到测试用的数据库后：

```
drop table if exists t_user;
create table t_user(
	name varchar(32)
);
```

A启动一个事务，查询t_user

```
start transaction;
select * from t_user;
//结果：
Empty set (0.00 sec)
```

B现在打开一个新cmd登录mysql，切到对应数据库，对数据进行如下操作

```
start transaction;
insert into t_user values('zhangsan');
```

A注意B事务还没有提交，但是在事务A中重新查询：

```
select * from t_user;
+----------+
| name     |
+----------+
| zhangsan |
+----------+
```

注意:测试serializable时，事务A在操作数据未提交时，事务B无法查询到数据，会显示为光标一直等待。

# Mysql笔记PART4

## 1.索引(index)

### 1.概念

在数据库表的字段上添加索引，是为了提高查询效率存在的一种机制。可以给一个或多个字段上索引。

索引相当于目录，可以定位到一个大概的位置，从而减少搜索范围。



```
select * from t_user where name = 'jack';
```

这条SQL语句会去扫描name字段，若name字段没有索引(nameIndex)，则进行全表扫描，即扫描所有name字段上的值，并进行比对。



mysql查询中主要是两种方式：

1.全表扫描

2.根据索引检索



注意：索引需要对元素进行排序，类比字典中的abcd..顺序，只有进行了排序，才能进行区间查找。

在mysql中索引也是排序的，而且这个索引的排序和TreeSet数据结构相同，是B-Tree数据结构。遵循左小右大原则存放，采用中序遍历取数据。

### 2.实现原理

注意：

1.任何数据库都会在主键上自动添加索引对象，在msql中，unique约束的字段也会自动创建索引对象。

2.在任何数据库中，任何一张表的任何一条记录在硬盘存储上都有一个硬盘的物理存储编号。

3.mysql中，索引是一个单独的对象，但在不同的存储引擎以不同的方式存在:

​	MYISAM中，索引存储在一个.MYI文件中

​	InnoDB中，索引存储在一个逻辑名称叫tablespace的当中

​	MEMORY，存在内存中

​	

例如：



![image-20220306155528293](./imgs\image-20220306155528293.png)

索引的数据结构：（简易版的，实际更加复杂）:

![image-20220306154808171](./imgs\image-20220306154808171.png)

### 3.什么情况下添加索引？

数据量庞大。

该字段经常出现在where后面总是被扫描。

该字段很少被DML（增删改）操作，DML之后索引需要重新排序。

注意：不要频繁创建索引，索引也是需要维护的；查询时尽量通过主键，unique约束的字段查询。

### 4.创建/删除索引

解释命令：

```
explain select * from emp where ename ='King';
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
| id | select_type | table | type | possible_keys | key  | key_len | ref  | rows | Extra       |
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
|  1 | SIMPLE      | emp   | ALL  | NULL          | NULL | NULL    | NULL |   14 | Using where |
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
rows=14说明进行了14次查找
```

创建索引：

```
create index emp_ename_index on emp(ename);
再次查看查找次数
explain select * from emp where ename ='King';
+----+-------------+-------+------+-----------------+-----------------+---------+-------+------+-----------+
| id | select_type | table | type | possible_keys   | key             | key_len | ref   | rows | Extra       |
+----+-------------+-------+------+-----------------+-----------------+---------+-------+------+-----------+
|  1 | SIMPLE      | emp   | ref  | emp_ename_index | emp_ename_index | 33      | const |    1 | Using where |
+----+-------------+-------+------+-----------------+-----------------+---------+-------+------+-----------+
rows=1
```

删除索引

```
drop index emp_ename_index on emp;
```



### 5.索引失效

#### 1）模糊查询导致失效

```
select * from emp where ename like '%T';
```

ename上即使添加索引，也不会走索引。

主要是模糊查询在开头匹配任意字符，无法进行索引。

**模糊查询尽量不要以%开头查询**

#### 2）使用or导致失效：

只有在or的两边的条件都有索引时才会使用索引，如果一个有索引一个没有索引，会导致索引失效

解决方法：采用union联合查询代替or

#### 3）使用复合索引时，没有使用左侧列查找

复合索引：两个字段或者多个字段联合起来添加一个索引

```
create index emp_job_sal_index on emp(sal,job);
```

其中sal是左侧，job是右侧，只有用左侧来查找才会使用索引，验证：

```
explain select * from emp where job='MANAGER';
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
| id | select_type | table | type | possible_keys | key  | key_len | ref  | rows | Extra       |
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
|  1 | SIMPLE      | emp   | ALL  | NULL          | NULL | NULL    | NULL |   14 | Using where |
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
explain select * from emp where sal = 800;
+----+-------------+-------+------+-------------------+-------------------+---------+-------+------+-------+
| id | select_type | table | type | possible_keys     | key               | key_len | ref   | rows | Extra        |
+----+-------------+-------+------+-------------------+-------------------+---------+-------+------+-------+
|  1 | SIMPLE      | emp   | ref  | emp_job_sal_index | emp_job_sal_index | 9       | const |    1 | Using where |
+----+-------------+-------+------+-------------------+-------------------+---------+-------+------+------+
```

#### 4）where中索引列参加运算

```
create index sal_index on emp(sal);
explain select * from emp where sal+1=800;
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
| id | select_type | table | type | possible_keys | key  | key_len | ref  | rows | Extra       |
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
|  1 | SIMPLE      | emp   | ALL  | NULL          | NULL | NULL    | NULL |   14 | Using where |
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
```

#### 5）where当中索引列使用了函数

```
create index ename_index on emp(ename);
explain select * from emp where lower(ename) = 'smith';
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
| id | select_type | table | type | possible_keys | key  | key_len | ref  | rows | Extra       |
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
|  1 | SIMPLE      | emp   | ALL  | NULL          | NULL | NULL    | NULL |   14 | Using where |
+----+-------------+-------+------+---------------+------+---------+------+------+-------------+
```

### 6.分类

优化的首要考虑因素就是索引。

单一索引

复合索引

主键索引

唯一性索引

...

考虑到索引的底层数据结构，在唯一性弱的字段上添加索引意义不大

## 2.视图（view）

### 1.创建/删除视图

创个新表：

```
create table dept2 as select * from dept;
select * from dept2;
+--------+------------+----------+
| DEPTNO | DNAME      | LOC      |
+--------+------------+----------+
|     10 | ACCOUNTING | NEW YORK |
|     20 | RESEARCH   | DALLAS   |
|     30 | SALES      | CHICAGO  |
|     40 | OPERATIONS | BOSTON   |
+--------+------------+----------+
```

创建视图：

```
create view dept2_v as select * from dept2;
//as后必须是DQL查询语句
```

删除视图：

```
drop view dept2_v;
```

### 2.视图的作用

我们可以面向视图进行增删改查，对视图的改变会导致对原表的改变。

 ```
 insert into dept2_v values (50,'SALES','BEIJING');
 select * from dept2;
 +--------+------------+----------+
 | DEPTNO | DNAME      | LOC      |
 +--------+------------+----------+
 |     10 | ACCOUNTING | NEW YORK |
 |     20 | RESEARCH   | DALLAS   |
 |     30 | SALES      | CHICAGO  |
 |     40 | OPERATIONS | BOSTON   |
 |     50 | SALES      | BEIJING  |
 +--------+------------+----------+
 
 delete from dept2_v;
 select * from dept2;
 Empty set (0.00 sec)
 ```

新建一个视图：

```
create view 
	emp_dept_view
as 
	select 
		e.ename,d.dname
	from 
		emp e
	join 
		dept d
	on 
		e.deptno = d.deptno;
		
select * from emp_dept_view;
+--------+------------+
| ename  | dname      |
+--------+------------+
| CLARK  | ACCOUNTING |
| KING   | ACCOUNTING |
| MILLER | ACCOUNTING |
| SMITH  | RESEARCH   |
| JONES  | RESEARCH   |
| SCOTT  | RESEARCH   |
| ADAMS  | RESEARCH   |
| FORD   | RESEARCH   |
| ALLEN  | SALES      |
| WARD   | SALES      |
| MARTIN | SALES      |
| BLAKE  | SALES      |
| TURNER | SALES      |
| JAMES  | SALES      |
+--------+------------+
```

实际开发的作用：假设有一条很长的sql语句（as后的语句），而这条语句在很多不同的位置重复使用，我们可以将它封装为一个视图。

优点：

可以简化开发；

并且有利于后续的维护，只需要修改视图映射的SQL语句即可；

 视图与表相同是存储在硬盘上的。

## 3.DBA命令

数据导出：导出一个sql文件

**管理员权限在cmd中：**

```
mysqldump -uroot -p almond>D:\almond.sql

mysqldump -uroot -p almond emp>D:\almond.sql
```

数据导入:

在mysql中，先创建好数据库，use该数据库：

```
source 路径;
```

## 4.数据库设计三范式（*）

第一范式：任何一张表必须有主键，每一个字段原子性不可再分。

第二范式：在第一范式的基础上，要求所有非主键字段完全依赖主键，不要产生部分依赖。

第三范式：在第二范式基础上，要求所有非主键字段直接依赖主键，不要产生传递依赖。

设计数据库表时，按照以上三范式进行，可以减少数据冗余。

### 1.第一范式

有主键，字段不可再分

![image-20220306174356948](./imgs\image-20220306174356948.png)

### 2.第二范式

非主键字段完全依赖主键，不要产生部分依赖

例如：

学会和老师是多对多关系

![image-20220306174721803](./imgs\image-20220306174721803.png)

修改：加联合主键

![image-20220306174736187](./imgs\image-20220306174736187.png)

不满足，张三依赖1001，王老师依赖001，张三和王老师都重复了。

应该使用三张表表示多对多的关系：A表，B表，AB关系表。

![image-20220306175144481](./imgs\image-20220306175144481.png)

![image-20220306175201254](./imgs\image-20220306175201254.png)

**记住多对多用三张表，两个外键表示**

### 3.第三范式

所有非主键字段必须直接依赖主键，不要产生传递依赖

![image-20220306175712476](./imgs\image-20220306175712476.png)

![image-20220306175828197](./imgs\image-20220306175828197.png)

**一对多，两张表，多的表加外键**

### 4.总结表的设计

一对多，两张表，多的表加外键

多对多，三张表，两个外键

一对一：一对一放到一张表太庞大时建议拆分为两张表

![image-20220306180600317](C:\Users\Alm0nd\Desktop\mysql笔记\mysqlNote\assets\image-20220306180600317.png)

login_id与登录信息表中id关联，并且设置unique约束。

有时，表可能存在冗余，因为sql的连接次数越多，效率越低。这是一种用空间换速度的方法。
