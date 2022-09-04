# 表

```
emp
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
dept
+--------+------------+----------+
| DEPTNO | DNAME      | LOC      |
+--------+------------+----------+
|     10 | ACCOUNTING | NEW YORK |
|     20 | RESEARCH   | DALLAS   |
|     30 | SALES      | CHICAGO  |
|     40 | OPERATIONS | BOSTON   |
+--------+------------+----------+
salgrade
+-------+-------+-------+
| GRADE | LOSAL | HISAL |
+-------+-------+-------+
|     1 |   700 |  1200 |
|     2 |  1201 |  1400 |
|     3 |  1401 |  2000 |
|     4 |  2001 |  3000 |
|     5 |  3001 |  9999 |
+-------+-------+-------+
```





# 1.取得每个部门最高薪水的人员名称

```
select deptno,max(sal) as maxsal from emp group by deptno

select e.ename,e.sal,e.deptno from emp e
join (select deptno,max(sal) as maxsal from emp group by deptno) t
on t.deptno=e.deptno and e.sal = t.maxsal;

```

# 2.哪些人的薪水在部门的平均薪水之上

```
//部门的平均薪资
select avg(sal) as avgsal,deptno from emp group by deptno;

select e.ename,e.sal,e.deptno from emp e
join (select avg(sal) as avgsal,deptno from emp group by deptno) t
on e.deptno=t.deptno
where e.sal>t.avgsal;
```

# 3.取得部门中（所有人的）平均的薪水等级

```
select avg(sal) as avgsal,deptno from emp group by deptno;

select t.deptno,s.grade from salgrade s
join (select avg(sal) as avgsal,deptno from emp group by deptno) t
on avgsal between s.losal and s.hisal;
（x）做成了平均薪水的等级

select e.sal,e.deptno,s.grade as gra from emp e
join salgrade s 
on e.sal between losal and hisal;

select e.deptno,avg(s.grade) as gra from emp e
join salgrade s 
on e.sal between losal and hisal
group by e.deptno;

```

# 4.不准用组函数（Max ），取得最高薪水

```
剔除sal最大值
select distinct e1.sal from emp e1
join emp e2
on e1.sal<e2.sal;
//e1.sal只要能小于一个e2.sal就能被连接，并显示,那么只有king不在其中

select sal from emp where sal not in (select distinct e1.sal from emp e1 join emp e2 on e1.sal<e2.sal);
```

# 5.取得平均薪水最高的部门的部门编号

```
select deptno from emp group by deptno order by avg(sal) desc limit 1;
```

# 6.取得平均薪水最高的部门的部门名称

```
select e.deptno,d.dname from emp e 
join dept d
on e.deptno = d.deptno
group by e.deptno 
order by avg(e.sal) desc 
limit 1;
```

# 7.求平均薪水的等级最低的部门的部门名称

```
select avg(e.sal) avgsal,e.deptno
from emp e
join salgrade s
on avgsal between s.losal and s.hisal
group by e.deptno;
(x)on avgsal出错，因为这时候还在做表连接，还没分组，既然没有分组，哪里会有分组函数的结果avgsal呢

部门的平均薪资：
select avg(e.sal) avgsal,e.deptno from emp e group by e.deptno;

select s.grade,t.deptno from salgrade s 
join (select avg(e.sal) avgsal,e.deptno from emp e group by e.deptno) t
on t.avgsal between s.losal and s.hisal;
+-------+--------+
| grade | deptno |
+-------+--------+
|     3 |     30 |
|     4 |     10 |
|     4 |     20 |
+-------+--------+
select s.grade,t.deptno,d.dname from salgrade s 
join (select avg(e.sal) avgsal,e.deptno from emp e group by e.deptno) t
on t.avgsal between s.losal and s.hisal
join dept d
on t.deptno=d.deptno;
+-------+--------+------------+
| grade | deptno | dname      |
+-------+--------+------------+
|     3 |     30 | SALES      |
|     4 |     10 | ACCOUNTING |
|     4 |     20 | RESEARCH   |
+-------+--------+------------+
select s.grade,t.deptno,d.dname from salgrade s 
join (select avg(e.sal) avgsal,e.deptno from emp e group by e.deptno) t
on t.avgsal between s.losal and s.hisal
join dept d
on t.deptno=d.deptno
order by s.grade asc
limit 1;
+-------+--------+-------+
| grade | deptno | dname |
+-------+--------+-------+
|     3 |     30 | SALES |
+-------+--------+-------+
```

# 8.取得比普通员工(员工代码没有在 mgr 字段上出现的) 的最高薪水还要高的领导人姓名

```
把老板找出来
select distinct e1.empno from emp e1
inner join emp e2
on e1.empno = e2.mgr;
(x)虽然没错，但这方法太复杂了
直接找领导的员工编号并且去重就ok了
select distinct mgr from emp where mgr is not null;

where mgr is not null！！！不加会出事儿，null和谁运算都是null

找出员工的最大薪资
select max(sal) from emp
where empno not in(select distinct mgr from emp where mgr is not null);

普通员工的最大薪资已经找出，比该薪资还大的只能是老板
select e3.ename from emp e3
where e3.sal>(select max(sal) from emp
where empno not in(select distinct mgr from emp where mgr is not null));
+-------+
| ename |
+-------+
| JONES |
| BLAKE |
| CLARK |
| SCOTT |
| KING  |
| FORD  |
+-------+
```

# 9.取得薪水最高的前五名员工

```
select sal from emp order by sal desc limit 5;
```

# 10.取得薪水最高的第六到第十名员工

```
select sal,ename from emp order by sal desc limit 5,5;
```

# 11.取得最后入职的 5 名员工

```
select hiredate,ename from emp order by hiredate desc limit 5;
```

# 12.取得每个薪水等级有多少员工

```
select s.grade,count(*) from emp e
join salgrade s
on e.sal between s.losal and s.hisal
group by s.grade;
```

# 13

```
13、面试题：
有 3 个表 S(学生表)，C（课程表），SC（学生选课表）
S（SNO，SNAME）代表（学号，姓名）
C（CNO，CNAME，CTEACHER）代表（课号，课名，教师）
SC（SNO，CNO，SCGRADE）代表（学号，课号，成绩）
问题：
1，找出没选过“黎明”老师的所有学生姓名。
2，列出 2 门以上（含2 门）不及格学生姓名及平均成绩。
3，即学过 1 号课程又学过 2 号课所有学生的姓名

1.
找黎明的课号
select cno from C where cname='liming';
找选了黎明课的学生学号
select sno from SC 
where cno=(select cno from C where cname='liming')

select S.sname from S
inner join SC 
on S.sno not in(select sno from SC 
where cno=(select cno from C where cname='liming'))
2.
找出学生和挂科数量
select sname,count(*) from S 
join SC
on S.sno=SC.sno
where SC.scgrade<60

select sname,avg(scgrade) as avggrade 
from (select sname,count(*) as num from S 
join SC
on S.sno=SC.sno
where SC.scgrade<60) t
join SC
on t.sno=SC.sno
where t.num>=2;

3.
select sname from S
join SC 
on S.sno=SC.sno
where SC.cno=1 and SC.con=2;
```

# 14.列出所有员工及领导的姓名

```
select a.ename,b.ename from emp a
inner join emp b
on a.mgr=b.empno;
(x)
king的mgr为null,不能跟b表的任何值匹配。最终会少一个值

select a.ename,b.ename from emp a
left join emp b
on a.mgr=b.empno;

```

# 15.列出受雇日期早于其直接上级的所有员工的编号,姓名,部门名称

```
select a.empno,a.ename,d.dname from emp a
join emp b
on a.mgr=b.empno
join dept d
on a.deptno=d.deptno
where a.hiredate<b.hiredate;
```

# 16.列出部门名称和这些部门的员工信息, 同时列出那些没有员工的部门

```
select d.dname,e.* from emp e
right join dept d
on e.deptno=d.deptno;
```

# 17.列出至少有 5 个员工的所有部门

```
select deptno from emp e
group by deptno
having count(*)>=5;
```

# 18.列出薪金比"SMITH" 多的所有员工信息

```
select sal from emp where ename='SMITH';
select * from emp where sal>(select sal from emp where ename='SMITH')
```

# 19.列出所有"CLERK"( 办事员) 的姓名及其部门名称, 部门的人数

```
部门人数
select y.deptno,count(*) as c from emp x
inner join dept y
on x.deptno=y.deptno
group by y.deptno;

select e.ename,d.dname,t.c from emp e
join dept d
on e.deptno=d.deptno
join (select y.deptno,count(*) as c from emp x
inner join dept y
on x.deptno=y.deptno
group by y.deptno) t
on t.deptno=e.deptno
where e.job='CLERK';
```

# 20.列出最低薪金大于 1500 的各种工作及从事此工作的全部雇员人数

```
select min(sal),count(*) as minsal,job from emp 
group by job
having min(sal)>1500;
```

# 21.列出在部门"SALES"< 销售部> 工作的员工的姓名, 假定不知道销售部的部门编号.

```
select deptno from dept where dname='SALES';

select e.ename from emp e
where e.deptno=(
select deptno from dept where dname='SALES'
);
```

# 22.列出薪金高于公司平均薪金的所有员工, 所在部门, 上级领导, 雇员的工资等级.

```
select avg(sal) as avgsal from emp;

select ename,deptno,mgr,s.grade from emp e
join salgrade s
on sal between s.losal and s.hisal
where sal>(select avg(sal) as avgsal from emp);

```

# 23.列出与"SCOTT" 从事相同工作的所有员工及部门名称

```
select e.ename,d.dname from emp e
join dept d
on e.deptno=d.deptno
where e.job=(select job from emp e where ename='SCOTT') and e.ename <>'SCOTT';
```

# 24.列出薪金等于部门 30 中员工的薪金的其他员工的姓名和薪金.

```
select distinct sal from emp where deptno=30;

select sal from emp 
where sal in(select distinct sal from emp where deptno=30) and deptno<>30;
```

# 25.列出薪金高于在部门 30 工作的所有员工的薪金的员工姓名和薪金. 部门名称

```
select e.sal,e.ename,d.dname from emp e
join dept d
on e.deptno=d.deptno
where e.sal>(select max(sal) from emp where deptno=30);
```

# 26.列出在每个部门工作的员工数量, 平均工资和平均服务期限

```
select d.deptno,d.dname,
ifnull(count(e.ename),0) as count,
ifnull(avg(e.sal),0) as avg,
ifnull(avg(timestampdiff(YEAR,e.hiredate,now())),0) as date
from emp e
right join dept d
on e.deptno=d.deptno
group by d.deptno;
```

# 27.列出所有部门的详细信息和人数

```
select d.*,ifnull(count(e.ename),0) as num from emp e
right join dept d
on e.deptno=d.deptno
group by d.deptno;
```

# 28.列出各种工作的最低工资及从事此工作的雇员姓名

```
select job,min(sal) as minsal from emp group by job;

select e.ename,t.minsal,t.job from emp e
inner join (select job,min(sal) as minsal from emp group by job) t
on e.sal=t.minsal and e.job=t.job
```

# 29.列出各个部门的 MANAGER( 领导) 的最低薪金

```
select min(sal),deptno from emp 
where job='MANAGER'
group by deptno;
```

# 30.列出所有员工的 年工资, 按 年薪从低到高排序

```
select ename,(sal+ifnull(comm,0))*12 as yearsal from emp
order by yearsal asc;
```

# 31.求出员工领导的薪水超过3000的员工名称与领导

```
select e.ename,m.ename from emp e
inner join emp m
on e.mgr=m.empno
where m.sal>3000;
```

# 32.求出部门名称中, 带'S'字符的部门员工的工资合计、部门人数

```
select deptno,dname from dept
where dname like '%S%';

select t.deptno,ifnull(sum(e.sal),0),ifnull(count(ename),0) from emp e
right join (select deptno,dname from dept
where dname like '%S%') t
on t.deptno=e.deptno
group by t.deptno;

```

# 33.给任职日期超过 30 年的员工加薪 10%.

```
update emp set sal=sal*1.1 where timestampdiff(year,hiredate,now())>30;
```



