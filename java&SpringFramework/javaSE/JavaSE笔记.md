

#  《java核心技术》重点整理

## 一. Java的基本程序设计结构（第三章）

### 1.一些注意事项

1. java对大小写敏感；

2. 类名是以大写字母开头，由多个单词组成的类名每个单词的首字母都要大写；

3. main方法必须是public；

4. 文档注释写法：

```java
/**

*/
```

### 2.基本数据类型（8种）

#### （1）整型（4种）

类型		存储需求空间			

int			 4 byte						

short        2 byte

long		  8 byte

byte	      1 byte



java每一种数据类型取值范围必须固定，c和c++针对不同处理器有不同的整型，这会导致在64位机器跑的程序放到32位机器就会溢出；



长整型数据有后缀L

十六进制有前缀0x

八进制有前缀0；



java7中，加上0b就可以写二进制数，如0b1001就是9。

还可以给数字字面量加_

1_000_000表示一百万；



---



#### （2）浮点类型（2种）

| 类型   | 存储需求空间 |
| ------ | ------------ |
| float  | 4 byte       |
| double | 8 byte       |

绝大多数情况都会使用double，因为float范围小精度低，除非要快速运算单精度数时会用float。

float类型数值后有后缀F，没有F的浮点数默认为double，也可以加后缀D表示double。



三种特殊的浮点数：（不适用于整型）

正无穷：正数/0  

负无穷：负数/0

NaN：不是一个数字

```java
 float a = 1;
 float o = 0;
 System.out.println(a/o);//Infinity
	
 int a = 1;
 int o = 0;
 System.out.println(a/o);
//报错java.lang.ArithmeticException: / by zero
```



> 浮点数不适用于金融计算，因为有偏差:

```java
System.out.println(2.0-1.1);//0.8999999999999999
```

为什么会有这种偏差呢？

正如十进制不能表示三分之一，二进制也不能准确表示一些数据，比如十分之一；

具体来讲：二进制可以表示1*2<sup>-x</sup>，这包括了1/2，1/2+1/4，1/2+1/4+1/8，... ；但是我们用尽所有的二进制数也无法凑出来一个0.1

想要精确表示，我们需要使用BigDecimal类:

```java
Double d1= 2.0;
Double d2 = 1.1;
BigDecimal b1 = new BigDecimal(d1);
BigDecimal b2 = new BigDecimal(d2);//debug发现b2的值仍然不正确，1.10000000....xxx，double类的构造器仍然丢失了精度
System.out.println(b1.subtract(b2));//0.8999999999999999,仍然无法解决
//两种解决方法都是利用了BigDecimal的String类的构造方法：
BigDecimal b3 = new BigDecimal(String.valueOf(d1));
BigDecimal b4 = new BigDecimal(String.valueOf(d2));
System.out.println(b3.subtract(b4));//0.9

BigDecimal b5 = new BigDecimal(Double.toString(d1));
BigDecimal b6 = new BigDecimal(Double.toString(d2));
System.out.println(b5.subtract(b6));//0.9
```



---



####  （3）char类型（1种）

使用UTF-16



---



#### （4）boolean类型（1种）

只有true和false两个值，注意java中boolean和整型不能转换，

意味着while（true）不能写成while（1）； 

### 3.常量

```java
final double p = 3.14;
```

final常量只能声明一次，一旦声明不能更改



我们经常声明一个类常量

在一个类的实例域中：

public static final double pi = 3.14

同一类实例的方法可以调用这个常量

### 4.运算符

```java
//>>>用0填充高位，>>用符号位填充高位，没有<<<运算符
```

### 5.数值类型转换

#### (1)自动类型转换

无信息丢失的转换：

byte-->short-->int-->long

char-->int

int--double

可能有信息丢失的转换

int-->float

long-->float

long-->double

#### （2）强制类型转换

```java
public void testR(){
    double a = 1.5;
    System.out.println( (int)a );//1
}
```

截断小数点来将浮点型转换为整型



若要对数据进行舍入，调用Math.round方法：

```java
public void testR(){
    double a = 1.5;
    System.out.println( Math.round(a) );//2
}
```

### 6.枚举类型

```java
@Test
public void testR(){
    enum Size {small,medium,large};
    Size s = Size.large;
    System.out.println(s);//large
}
```

### 7.字符串

Almond

#### （1）提取字符串的字串

substring(a,b)

a:第一个开始摘录的位置下标，从0开始

b:停止摘录的下标

”顾头不顾尾“

```java
@Test
public void testR(){
    String name = "Kirisame Marisa";
    String firstname = name.substring(0,8);
    System.out.println(firstname);//Kirisame
    
    String fn = name.substring(0,7);
    System.out.println(fn);//Kirisam
}
```

#### （2）字符串连接

str1+str2

#### （3）不可变的字符串

字面量定义：String str =  "apple";

new+构造器定义：String str = new String("apple");



###### 1.字面量定义：

当一个字符串被字面量定义时，会先到常量池中寻找，如果有则直接指向这个字符串，若没有则在常量池中创建新字符串。

编译器可以让字符串共享，当一个常量池字符没有引用时，它就会自动被垃圾回收；

```java
@Test
public void testR() {
    String a = "java";
    String b = "java";
    System.out.println(a==b);//true
    //说明a，b指向同一个字符串
    
    //当b改变
    b = "json";
    //a保持不变，b开辟了新字符串
    System.out.println(a);//java
    System.out.println(b);//json
}
```

a，b引用的都是字符串的第一个地址（其中有关于字符串长度的信息），而不是每个字符的地址都有。也就是说，a，b相当与char型指针，而不是char*数组；



###### 2.new+构造器定义：

注意：字符串连接时如果出现连接一个已有的String类型变量，无论它是什么方式定义的，连接结果都会转换为对象

```java
String a = "java"; 
String b = a + "se";//运算中有变量a
String c = "javase";
System.out.println(b==c);//false

String d="java"+"se";
System.out.println(d==c);//true
```



和对象的定义一样，在栈中创建引用，在堆中开辟新空间，其中有指向常量池的引用；

new+构造器在堆中创建了一个对象，这个对象里有一个char*，指向常量池中字符串的首地址；



比较两种方式的引用

```java
@Test
public void testR() {
    String firstname1 = "Kilisame";
    String firstname2 = "Kilisame";
    System.out.println(firstname1 == firstname2);//true

    String lastname1 = new String("Marisa");
    String lastname2 = new String("Marisa");
    System.out.println(lastname1==lastname2);//false
    //两者在常量池中的地址还是相同的，但这里比较的是堆中实例的地址
}
```



例题：

```java
public class ReviewTest {
    String str1 = "apple";
    char[] c1={'a','p','e','x'};
    public void change(String str1,char[] c1){
        str1 = "test ok";
        c1[0] = 'x';
    }
    public static void main(String[] args) {
        ReviewTest rt = new ReviewTest();
        rt.change(rt.str1,rt.c1);
        System.out.println(rt.str1);
        System.out.println(rt.c1);
    }
}
```

结果：apple

​			xpex

原因：str1传入change方法，创造局部的str1指向常量池，将str1重新指向”test ok“，此时已经与外部的str1不同了（字符串的不可变性）；而char[]的局部变量引用到了同一个堆空间，所以可以修改。

#### （4）检测字符串是否相等

不能写成==

区分大小写：

```java
s.equals(t);
```

不区分大小写：

```java
s.equalsIgnoreCase(t);
```

#### (5)空串与null串

空串：

```java
String s ="";
```

空串有串长度（0）以及内容（空）



null串：指这个变量还没有跟任何字符串相关联；

我们常常需要检查一个串非空非null

```java
if(str != null && str.length() != 0)
//或者，不能写成！=
if(str != null && str.equals(""))
```

写成==的后果

```java
public class ReviewTest {
    public static void main(String[] args) {
        String s =new String("");
        System.out.println(s!="");//true
    }
}
```

### 8.数组

#### （1）数组拷贝

一般想到的拷贝方式是直接赋值，但是这会使两个变量指向同一个数组，当一个变量改变数组元素的值时，另一个也会改变

```java
int[] list = {1, 23, 4};
int[] copylist = list;
copylist[1] = 2;
System.out.println(list[1]);//2
```

要想拷贝数组就要使用copyTo()方法

```java
int[] list = {1, 23, 4};

int[] copylist = Arrays.copyOf(list,list.length);//参数：要拷贝的数组名，拷贝后新数组的长度
//当新数组长度>被拷贝数组的长度，多出的部分赋默认值
//数值赋值0，布尔赋值false。若新数组比被拷贝数组短，则只拷贝前面的数值
System.out.println(copylist == list);//不是同一引用

for (int i = 0;i<list.length;i++){
    if(list[i]==copylist[i]){
        System.out.println("第"+i+"个元素相等");
    }
}//元素都相等
```

#### （2）数组排序

使用Arrays类中的sort()方法,是快速排序；

## 二.对象与类（第四章）

### 1.面向对象

#### （1）类

类是构造对象的蓝图或者模板，由类构造对象的过程称为创建类的实例；

对象中的数据称为实例域，操作数据的过程称为方法。

对于每个特定的对象都有一组特定的实例域值，这些值的集合称为对象的当前状态，这个状态是可能改变的。

#### （2）对象

三大特征：

对象的行为：可以对对象做什么

对象的状态：当施加方法时，对象如何响应

对象标识：辨别相同状态和行为的对象

#### （3）类之间的关系

依赖：（uses-a）一个类中的方法需要使用另一个类的对象。尽可能避免这种情况。

聚合：（has-a）一个类的对象包括另一个类的对象

继承：（is-a）子类继承父类

### 2.使用预定义类

#### （1）对象和对象变量

```java
Data deadline;
```

deadline是一个对象变量，此时还没有引用任何对象，如果此时让deadline调用方法，会编译出错。

可以创建一个新对象，也可以引用已经有的对象；

对象变量只是指向在堆中的对象，并不是含有一个对象；



```
Data deadline = null;
deadline.toString();//Exception in thread "main" java.lang.NullPointerException
```

如果对null运用方法则会runtime exception

#### （2）GregorianCalendar类

时间是从1970.1.1 00:00:00到现在的毫秒数，1970.1.1 00:00:00称为纪元

GregorianCalendar类继承了一个Calendar类

### 3.用户自定义类

#### （1）权限修饰符

public：所有类的任何方法都可以调用这些方法

private：只有这个类自身可以访问这些实例域，建议实例域用private

#### （2）方法参数类别

隐式参数：这个类的对象的实例域中的参数

显式参数：方法中外界传入的参数

``` java
public void raiseSalary(double amount){
	this.salary += amount;//salary是隐式参数，amount是显式参数
}
```



参数salary只有raiseSalary控制，一旦这个参数出现错误，直接找raiseSalary就可以了，这就是实例域用private的好处。

#### （2）封装性的设计原则

为了封装性，我们最好提供：

1.private的实例域

2.public的域访问器方法（get）

3.public的域修改器方法（set）

这样任何方法都可在类的内部实现，不用修改其他代码，比如把name改为lastname和firstname，只需要把get方法改变即可。

还有一个好处，set方法可以检查数据的正确性，比如检查salary是否小于0。



注意：不要写类中不可变引用对象的get方法。

string是不可变的可以写get（）方法：

```java
public class Employee {
    private String name;

    public String getName() {
        return name;
    }

    public Employee(String name) {
        this.name = name;
    }

    public static void main(String[] args) {
        Employee e = new Employee("Morisa");
        String name = e.getName();
        name = "Reimu";
        System.out.println(e.getName());//Morisa
    }
}
```

如果写的是其他对象的get方法，我们可以直接从外部获得这个内部对象

``` java
Date h = e.getHireDay();
```

h和e.HireDay指向同一个变量，这会导致封装性的破坏。



如果需要返回这个对象，我们可以返回这个对象的克隆

``` java
return hireDay.clone();
```



#### （4）基于类的访问权限

方法可以访问这个类的所有对象的私有数据。注意：主语是方法，而不是该类的对象。

``` java
e.name.equals( boss.name )
```

这里employee对象就访问了它的另一个对象的私有数据，boss也是employee的子类，子类当然也是employee类

#### （5）final实例域

可以把实例域定义为final，必须确保final的数据在构造器构造对象后会被赋值，并且以后不会对它修改



final大多用于基本类型以及不可变类型（String）;

不适合引用类型:

``` java
private final Date hireDay;
```

hireDay仅仅是一个引用hireDay对象的变量罢了，final只是不能修改这个变量值，但我们可以直接改这个引用指向的对象

### 4.静态域与静态方法

#### （1）静态域

例如：给职员每个人一个固定的id

``` java
private static int ID;
```

ID属于类，不属于任何这个类的对象

#### （2）静态常量

Math类中有：

``` java
public static final PI = 3.14.....;
```

这样想调用PI值的时候，直接用Math.PI就可以了不用创建Math对象

#### （3）静态方法

静态方法和静态域静态常量一样是在类创建时创建，因此只能访问这些静态域或者静态常量。

建议用类名而不是对象名调用静态方法。

#### （4）工厂方法

通过静态方法来实现工厂模式

``` java
Car EV = CarFactory.getElectricVehicle();
Car MV = CarFactory.getMotorVehicle();
```

工厂模式的好处：构造器必须和类名一致，而一个工厂可以产出不同类的对象。

### 5.方法参数

按值调用：方法调用的是调用者提供的值；

按引用调用：方法调用的是调用者提供的变量地址；

java中总是采用按值调用，方法得到的是参数变量的拷贝。

不过如果参数是引用类型，方法会得到这个引用类型的地址的**拷贝**，所以可以对其进行操作。

例如：

``` java
Employee a = new Employee();
Employee b = new Employee();
swap(a,b)
```

``` java
//这时建立对a，b的拷贝，x=a，y=b
Employee temp = x;
x = y;
y = temp;
//现在x，y确实调换了，但是外部的a，b并没有调换
```

如果是引用调用，x，y的值也交换了



1.方法不能修改一个基本数据类型的参数

2.方法可以修改一个对象变量的状态

3.方法不能让一个对象参数引用另一个对象

### 6.对象构造

#### （1）重载

多个方法，具有相同的名字，不同的参数，这就是重载。

编译器找出这个方法提供参数与调用这个方法提供值的类型进行匹配，如果找不到对应的方法或者发现多个方法匹配，会报编译错误。

#### （2）默认域初始化

默认下，数值赋值0，布尔赋值false，对象引用为null。

建议最好赋好初始值，不要用默认初始化。

#### （3）无参数的构造器

如果没有提供构造器，系统会自动生成一个空参构造器，这个构造器会把所有值赋为默认值。

#### （4）显示域初始化

可以在类在定义时就赋初始值

``` java
class Employee{
    private String name = "";
}
```



在执行构造器之前，先执行赋值操作，可以调用方法对域进行初始化；

``` java
class Employee{
    private static int id;
    private int id = assignId(); 
}
```



利用静态方法赋值

```java
public class Employee {
    private static int nextId;//在类中赋值一次为0
    private int id = assignId();//通过nextId给id赋值
    private static int assignId(){
        int r = nextId;
        nextId++;
        return r;
    }
}
```

实现了类的对象中id从0开始自增1；



#### （5）参数名和实例域名重名

this.name=name;

this.参数名来访问对象实例域，参数名来表示显式参数；

#### （6）调用另一个构造器

如果构造器的第一个语句形如this(....)，他将会调用同一个类的构造器。

#### （7）初始化块

初始化块

``` java
public class Employee {
    private double salary;
    {
        salary = 1000;//初始化块
    }
    public Employee(double salary) {
        this.salary = salary;
    }
}
```

初始化块会在构造器之前执行（无论他放在构造器前还是构造器后）

```java
public class Employee {
    private double salary;
    public Employee(double salary) {
        this.salary = salary;
    } 
    {
        salary = 1000;
    }
```

这样与上面没有任何差别，最后salary都是构造器赋值的结果，说明代码块先执行。

这种机制不是必须的，也不常见，可以直接吧初始化块放在构造器中。



调用构造器的具体执行步骤：

1.数据域进行默认初始化

2.按照类代码写的次序依次执行所有域初始化语句，初始化代码块

初始化语句：

```java
private double salary = 1000;
```

3.构造器的第一行调用了另一个构造器则执行另一个构造器

4.执行构造器主体

### 7.包

Sun公司建议以公司的因特网域名的逆序作为包名，如

com.horstmann.corejava

#### （1）类的导入

一个类可以使用所属包的所有类，以及其他包的共用类，我们可以采用两种方式访问另一个包中的类。

1.写包名

``` java
java.util.Date today = new java.util.Data();
```

2.导入包，使用的时候直接写，不用写包名

``` java
import java.util.Data;
```



如果同时要用重名的类，比如要用java.util.Date和java.sql.Date,我们只能加完整的包名，即1.

#### （2）静态导入

import不仅可以导入类，还可以导入静态方法和静态域

``` java
import static java.lang.System.*
```

这样就导入了静态方法和静态域，可以不加类名前缀

out.println("goodbye");

#### （3）将类放入包中

包的名字在源文件的开头

```java
package com.Almond.javaReview;
```

如果没有这个包名，这个源文件的类会放在一个默认包中，它是一个没有名字的包



注意：编译器在编译文件时不会检查目录结构，也就是文件不在com\Almond中也能通过编译，但是运行时会报错，虚拟机无法找到文件。

#### （4）包作用域

类的访问修饰符：

public：可以被任何类使用

private：只能被自己调用

缺省：可以被同一个包内的所有方法访问

默认情况下，任何人都可以向包中添加更多的类，包不是一个封闭的实体。如果把一个包封闭起来，就不能继续向这个包中添加类了。

##  三.继承（第五章）

### 1.类，子类，超类

#### （1）子类重写父类方法

超类中的方法对子类未必适用，比如Emplyee不一定适用于Manager，为此子类Manger需要一个新的方法来覆盖这个父类的方法。



例如：Manager中的salary需要薪水加股票，看起来应该这样实现

``` java
public double getSalary(){
    return salary + bonus;//这样行不通
}
```

**即使是子类，也无法访问父类的私有域，**我们应该通过一个get()方法来访问

```java
@Override
public double getSalary(){
    return getSalary()+bonus;//java.lang.StackOverflowError
}
```

因为编译器不知道调用的是子类的还是父类的getSalary()方法

```java
@Override
public double getSalary(){
    return super.getSalary()+bonus;//成功了
}
```



注意：有人认为this和super都是对对象的引用，实际上super不是一个对象的引用，不能将super赋给另一个对象变量，它仅仅是指示编译器调用父类方法的特殊关键字。

super可以用来调用超类的方法和超类的构造器，这点和this相似。



一个对象变量可以指向多种实际类型的现象称为多态，在运行时能够自动选择调用哪个方法的现象称为动态绑定。

#### （2）多态

置换规则：可以在程序出现超类的任何地方用子类进行替换。（当然这个对象必须是子类构造的）

``` java
Emplyee e;
e = new Manager();//父类的引用指向的是一个子类的对象
```

用处：

``` java
Manager boss = new Manager();
Employee [] staff = new Employee[3];//Employee数组
staff[0] = boss;//子类对象可以装入父类数组中
```

在调用子类方法时注意，要通过子类对象名调用，而不是数组调用。这事因为staff[0]声明的是Employee类型，而setBonus()不是这个类型的方法。

```java
System.out.println( m.getBonus() );//1500
System.out.println( e[0].getBonus() );//java: 找不到符号
```



不能把一个超类的方法引用给一个子类的对象。



注意：子类数组可以自动转换为父类数组，因为子类对象也都是父类对象。

```java
Manager[] managers = new Manager[10];
for (int i = 0; i < managers.length; i++) {
            managers[i] = new Manager();
        }
Employee[] e = managers;//ok
```

切记，此时转换后的e和managers指向同一些对象，而e是可以接纳Employee类型的对象的。



#### （3）动态绑定

调用对象方法的过程：

1.编译器查看对象声明类型和方法名。

例如：调用p.f();编译器会列举这个对象声明类型的名为f的方法和它超类的名为f的public/protected方法.

2.查看调用方法时提供的参数类型，例如：同时有f(int)和f(String)，编译器会根据参数判断要调用哪一个方法。

3.**如果是private,static,final方法（这些方法不能够被子类继承）以及构造器方法**，编译器自动知道该调用的是哪个具体的方法。

**静态绑定/前期绑定：在编译时完成的绑定**

**后期绑定：运行时实现动态绑定**。

4.程序运行，而且采用动态绑定调用方法时，虚拟机一定调用的是与p所引用的对象的类最匹配的那个类的方法。

例如：p实际类型是D，D是C的子类。如果D中有匹配的方法，虚拟机直接调用，如果没有，在C中寻找。



整个调用方法的程序的例子：e.getSalary();//Employee类的e

1.解析对象和方法名

2.检查是否重载/no

3.不是private，static，final方法-->采用动态绑定

4.

（1）列举Employee类以及其子类的所有方法

（2）搜索定义getSalary()的类，此时明确该用哪个方法了

（3）虚拟机调用方法	



注意：子类在重写父类的方法时，**子类方法的可见性大于父类方法的可见性**

父类如果是public，子类必须是public的，否则报编译错误。

#### （4）final类和方法

想要阻止一个类定义子类的时候，我们将这个类声明为final类型，不允许被继承的类称为final类。

方法：final方法是不允许被子类覆盖的方法（final类中的方法自动为final方法）

注意：final可以修饰实例域，这个是指构造对象后该实例域的值不允许改变了，final类中的实例域仍然是可变的，不受其影响。



内联：如果一个方法没有被重写而且很短，编译器可以对它进行优化处理，这就是内联。

当有一个没有被重写，很简短，被频繁调用的方法，编译器很可能对其进行内联操作。

具体含义是：没有重写的方法（比如final方法等）将方法像主函数一样执行，而不用想正常方法执行一样需要参数压栈，方法压栈然后再退栈，这样可以节省资源。

一个内联的方法在被重写以后，这个内联将会取消。

#### （5）强制类型转换

使用强制类型转换的唯一原因：在暂时忽视对象的真实类型以后，使用对象的全部功能。

``` java
Manager boss = new Manager();
Employee [] staff = new Employee[3];//Employee数组
staff[0] = boss;//暂时忽视对象的真实类型
//....
Manager m = staff[0];// 现在要使用Manager的全部功能
```

可以将一个父类的变量指向子类对象

将一个子类变量指向父类对象，需要强制类型转换，而且这个父类对象的实际类型得是这个子类。

在强制类型转换前必须先看看类型是否匹配

``` java
if(staff[0] instanceof Manager){
    boss = (Manager) staff[0];
}
```

 注意，x instanceof A, x如果是null，仍然是false而不是异常。因为x不属于任何类型，自然不是A类型的实例。

#### （6）抽象类

抽象方法：不在类中给出具体实现而让子类重写实现的方法。

为了提高程序的清晰度，包含一个或者多个抽象方法的类本事应该被声明为抽象的。

抽象类中可以包括具体数据和具体方法。

继承抽象类：实现了一部分抽象方法，但还有未实现的抽象方法的，子类必须也是抽象类；

给出所有抽象方法实现的定义成非抽象的类。

不含抽象类的方法也可以定义为抽象类。

抽象类不能实例化，但是可以定义抽象类的变量，这个变量只能引用非抽象子类的对象。

#### （7）受保护访问

任何声明为private的内容对于其他类是不可见的，子类也不能访问父类的私有域。

要想允许子类访问父类的某个域，父类的这个域应该声明为protected的，

但是在实际运用中，要谨慎使用protected，因为假设我们在一个超类中定义了protected域，子类就可以由这个受保护域扩展新类，如果要对这个受保护域更改，所有使用这个类的子类都要更改，这违法了OOP的数据封装原则。



四种权限修饰符：

public：对所有类可见

protected：本包和子类可见

缺省：本包可见

private：这个类可见，子类也不可见

### 2.Object类：所有类的父类

如果一个类没有指明超类，Object默认为他的超类

java中只有基本数据类型不是对象。

基本数据类型：byte，short，int，long，float，double，boolean，char

数组，字符串这些都是对象，数组无论是对象数组还是基本类型数组都是对象。

#### （1）equals方法

Object类中的equals用于比较两个对象是否相等，比较的是两个对象是否有相同的引用。

一般情况下我们可能有这样的需求，只是检查两个对象的状态是否相同，而不是要求他们指向同一个对象，这时需要重写equals方法。

```java
@Override
public boolean equals(Object o) {// 注意这里类型只能是Object
    if (this == o) return true;//快速比较一下
    if (o == null || getClass() != o.getClass()) return false;
    Employee employee = (Employee) o;
    return Double.compare(employee.salary, salary) == 0;
}
```

如果这个类有除了object类的其他父类，在子类中定义equals方法时，需要先调用超类的equals方法（超类也提供了对Object的equals的重写），如果超类域都相等，再比较子类的实例域。

```java
@Override
public boolean equals(Object o) {
    if (this == o) return true;
    if (o == null || getClass() != o.getClass()) return false;
    if (!super.equals(o)) return false;
    Manager manager = (Manager) o;
    return Double.compare(manager.bonus, bonus) == 0;
}
```

#### （2）相等测试与继承

##### 1.equals中怎么判断类型？

如果发现类不匹配，equals方法返回false，但许多人使用instanceof进行equals中的类型检测

``` java
if (!(otherObject instanceof Emplyee)) return false;
```

m是manager类，instanceof识别子类时也为true

```java
System.out.println( m instanceof Employee);//true
```



但这样无法解决otherObject是子类的时候，而且会导致一些麻烦：

Java要求equals满足:(x,y,z都是非空引用)

自反性：x.equals(x)为真

对称性：x.equals(y),y.equals(x) 结果应该相等

传递性：x.equals(y),y.equals(z)为真，x.equals(z)也应该为真

一致性：x，y不变时，多次调用 x.equals(y)的结果应该相等

对于任何非空引用x，x.equals(null)返回false；



 关于对称性，如果不是同一个类时，对于Manager的m和Employee的e：

``` java
e.equals(m);//true
m.equals(e);// 按照instanceof判断类型时时fals
```

总结：

==1.如果子类各自拥有各自的相等的概念，就需要用getClass==

==2.如果父类决定相等的概念，可以用instanceof==



##### 2.equals方法思路

完美的equals方法写法：显示参数为o

1.快速判断一下是否指向同一个对象

2.判断o是否是null

3.比较类型：（1）如果比较的概念在每个子类都有所不同，使用getClass()

(2)如果比较的概念由父类定义，各个子类都遵守这个比较，使用instanceof

4.(1)如果this是子类，父类不是object且父类已经重写了equals方法，调用super.equals(o)

4.(2)将o转换为this类型

5.实例域比较：使用==比较基本数据类型，equals比较对象域



#### （3）hashcode方法

##### 1.hashcode是什么？

散列码是根据对象的参数等导出的一个整型数。（可以是负数且最好均匀）

Object类中定义了hashcode方法，所以每个对象都有一个默认的散列码，值为对象的存储地址。

##### 2.为什么重写equals方法必须重写hashcode方法？

重写equals方法就必须重写hashcode方法，hashcode主要是给HashMap使用的，hashmap会根据对象（键）生成一个hashcode（值），但是两个不同的对象也有可能索引在同一个值上，这时需要在这个值上建立一个链表。

由于我们是在哈希表中寻找元素，比较对象是否相等直接用hashcode的索引比较就行了，但如果遇到了哈希图上的链表就需要用equals方法逐一比较。

例：

```java
public class Key {
    int id = 1;

//    @Override
//    public boolean equals(Object o) {
//        if (this == o) return true;
//        if (o == null || getClass() != o.getClass()) return false;
//        Key key = (Key) o;
//        return id == key.id;
//    }

//    @Override
//    public int hashCode() {
//        return Objects.hash(id);
//    }

    public static void main(String[] args) {
        Key k1 = new Key();
        Key k2 = new Key();
        HashMap hashMap = new HashMap();
        hashMap.put(k1,"得到k1");//只装入k1

        System.out.println(k1.hashCode()==k2.hashCode());//false
        System.out.println( hashMap.get(k2) );//null
    }
}
```

当我们没有重写hashcode和equals时，hashcode默认是地址值，k1，k2的地址不同，hashcode自然不同。

```java
public class Key {
    int id = 1;

//    @Override
//    public boolean equals(Object o) {
//        if (this == o) return true;
//        if (o == null || getClass() != o.getClass()) return false;
//        Key key = (Key) o;
//        return id == key.id;
//    }

    @Override
    public int hashCode() {
        return Objects.hash(id);
    }

    public static void main(String[] args) {
        Key k1 = new Key();
        Key k2 = new Key();
        HashMap hashMap = new HashMap();
        hashMap.put(k1,"得到k1");

        System.out.println(k1.hashCode()==k2.hashCode());//true
        System.out.println( hashMap.get(k2) );//null
    }
}
```

为什么还是null？因为这时找到了同一个哈希索引，但是此时没有重写equals方法，使用的是Object的方法，比较的是地址值。

```java
public class Key {
    int id = 1;

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Key key = (Key) o;
        return id == key.id;
    }

    @Override
    public int hashCode() {
        return Objects.hash(id);
    }

    public static void main(String[] args) {
        Key k1 = new Key();
        Key k2 = new Key();
        HashMap hashMap = new HashMap();
        hashMap.put(k1,"得到k1");

        System.out.println(k1.hashCode()==k2.hashCode());//true
        System.out.println( hashMap.get(k2) );//得到k1，因为k1和k2是完全相等的，equals比较的方法自然相同了
    }
}
```

##### 3.java 7的改进

使用null安全的方法Objects.hashCode(),如果参数是null，返回值为0；

更好的做法是，需要组合多个散列值时，调用Objects.hash(参数1，参数2，……，参数n)

注意这里的Objects是一个类的名字，而且也不是Object。



equals方法和hashCode方法：

若x和y equals则x，y哈希值也相同

若x和y 不equals则x，y哈希值可能相同

若x，y 哈希值相同，x，y不一定equals

若x，y哈希值不同，x，y一定不equals

#### （4）toString方法

返回对象值的字符串。

只要对象与一个字符串和+相连接则自动调用toString方法。

所以 ""+x 相当于 x.toString()。

数组继承了object的toString方法，会按照旧的方法打印；

可以调用Arrays.toString(数组名)方法调用数组的toString打印

### 3.泛型数组列表

#### （1）数组列表类

##### 1.泛型

如何解决内容超出数组的大小的问题：使用ArrayList类，他具有自动调节数组大小的方法。

ArrayList是一个采用类型参数（<>）的泛型类，要想指定数组列表保存的参数类型，ArrayList<参数类>即可。

``` java
ArrayList<Employee> staff = new ArrayList<Employee>();
```

javaSE7中，可以省略后面的类型参数

``` java
ArrayList<Employee> staff = new ArrayList<>();
```

根据前一个<>自动推断类型：

如果赋值给一个变量，编译器会检查这个变量的泛型类型

传递到一个方法，编译器会检查参数的泛型类型

从一个方法返回，编译器会检查这个方法的泛型类型

然后将这个类型放在后面的<>中

这里就是把前面变量的泛型类型放在后面的<>中。



##### 2.扩展空间

在ArrayList装满的情况下，再添加新元素，ArrayList会创建一个更大的数组，将之前的数组拷贝至新数组中。

如果能估算出数组可能存储的数量，可以再填充数组前调用

``` java
staff.ensureCapacity(10);//最小容量10
```

这个方法调用将分配能装下10个对象的数组列表

还可以把初始容量传递给ArrayList构造器

```java
ArrayList<Employee> staff = new ArrayList<>(100);
```



==注意：这里的估算数组容量和数组初始化是不一样的==

new Employee[100];大小固定是100

new ArrayList<>(100);容量是100

容量是100的元素数组列表是有存100个的潜力，超过100也可以存。实际上，数组列表初始化时还没有存任何元素

```java
ArrayList<Employee> staff = new ArrayList<>(100);
System.out.println( staff.size() );//0
Employee[] es = new Employee[100];
System.out.println( es.length );//100
System.out.println( es[0] );//null
```

一旦确定数组列表大小已经固定，可以调用trimToSize方法。这个方法可以把存储区域大小调成为当前元素数量所需要的大小，剩余空间被垃圾回收。确认大小后再装元素需要花费时间移动存储块，不要这样做。

#### （2）访问数组列表元素

使用get，set方法实现访问和改变数组元素的操作，而不是[]语法

设置第i个元素，从0开始

``` java
staff.set(i,tom);
```

==只有i小于数组列表的大小时才能调用list.set(i,x)，==

set方法只能使用在已经填充元素的位置，不能添加元素。使用add方法添加元素



获得数组列表中的元素：

```java
strings.get(index)；//等价于strings[index]
```



注意：没有泛型类的ArrayList类提供的get方法返回Object类型，因此需要对返回值进行强制类型转换，如果向这个数组列表中装其他类的对象，编译不会报错，但运行时可以会出错。

介于兼容性，编译器在编译时检查了数组列表的类型后，如果没有发现错误，会将这个数组列表转换为Object类型的ArrayList。

### 4.对象包装器和自动装箱

对象包装器类：Ineger,Long,Float,Double,Short,Byte(前六个派生于公共的超类Number)

, Charater,Void,Boolean.

对象包装器类是final的，不能定义子类

定义：

```java
ArrayList<Integer> list = new ArrayList<>(10);
```

由于每个值包装在对象中，ArrayList<Integer>的效率远不如数组，所以应该用它造小型数组



java5中更新了自动装箱

```java
list.add(3);
```

等价于

```java
list.add(Integer.valueOf(3));
```

自动拆箱：

```java
int n = list.get(0);
```

等价于

```java
int n = list.get(0).intValue();
```

在算数运算中也可以使用自动装箱和自动拆箱，例如：

``` java
Integer n = 3;
n++;
```

过程：拆箱，运算，装箱

==注意：==

==运算符检测的是两个变量指向的是不是同一个对象，一般都不成立，但是：

``` java
for (int i = -128;i<=127;i++){
            Integer x = i;
            Integer y = i;
            if(x==y){
                System.out.println("Integer"+i+"相等");//全部相等
            }
        }
        Integer a = 128;
        Integer b = 128;
        System.out.println( a==b );//false
```

介于[-128，127]的short和int的数被包装类包装到了固定的对象，所以比较地址将会相等。



装箱和拆箱是编译器认可的，而不是虚拟机。编译器在生成类的字节码时，插入必要的方法调用。虚拟机只是执行这些字节码。

### 5.参数数量可变的方法

 ``` java
 public PrintStream printf(String fmt,Object... args)
 ```

这里的...是java代码中的一部分，表示接受任意数量的Object类型的对象，并将他们存在args数组中。

fmt是要打印的字符串，其中有格式说明符（%d等），用args[]中的内容替换。

### 6.枚举类

``` java
public enum Size{SMALL,MEDIUM,LARGE}
```

定义的类型Size是一个类，有三个实例

比较两个枚举类时，不要调用equals，直接==就行

所有的枚举类都是Enum类的子类。他们继承的方法有：
toString方法，与toString方法相反的valueOf方法:

```java
enum Size {s,m,l};
Size s = Enum.valueOf(Size.class,"m");//Enum从Size类拿出m赋给s
System.out.println(s);//m
```

values方法，返回包含一个枚举类型全部枚举值的数组

```java
Size [] sizes = Size.values();
System.out.println(Arrays.toString(sizes));//[s, m, l]
```

ordinal方法返回枚举常量在枚举类的位置

```java
System.out.println(Size.s.ordinal());//0
```

==注释==

和Class类一样，Enum类省略了一个类型参数，实际上的Size应该是Enum<Size>

### 7.反射

反射：分析类能力的程度

#### （1）Class类

Object类中的getClass()返回一个Class类的实例

Class类的方法：

##### 1.getName

getName：返回类的名字（在一个包里时，包名也会作为名字的一部分）

```java
Employee e = new Employee();
System.out.println(e.getClass().getName());//com.Almond.javaReview.Employee
```

##### 2.forName

forName：获得类名对应的Class对象，（需要throws异常对象）

```java
String className = "com.Almond.javaReview.Employee";
Class cl = Class.forName(className);
```

##### 3.名字.class

获得Class类的对象：T.class

一个Class对象实际上表示的是一种类型，这个类型未必是一种类

int不是类，但是int.class是一个Class类型的对象。

##### 4.newInstance

快速创建一个类的实例

```java
Employee e1 = e.getClass().newInstance();
```

e1拥有与e相同类型的实例。newInstance方法通过默认构造器创建新初始化的对象，如果没有默认构造器，则抛出一个异常。

#### （2）捕获异常

##### 1.try-catch

将可能出错的代码放入try块，catch提供处理器代码

如果有异常，进入catch处理；如果出try块仍然没有异常，跳过catch块。

#### （3）利用反射分析类的能力

Class类中的

getFields:返回类提供的public域

getMethods:返回类提供的方法

getConstructors:返回类提供的构造器数组

==其中包括超类的共有成员。==

getDeclareFields:返回全部域

getDeclareMethods:返回全部方法

getDeclareConstructors:返回全部构造器

包括私有和受保护成员，==但不包括超类成员。==

#### （4）运行时使用反射分析对象

```java
Employee e = new Employee();
Class cl = e.getClass();
Field field = cl.getDeclaredField("salary");//salary是私有的
Object o = field.get(e);//IllegalAccessException
```

java允许查看任何对象有哪些域，但是不能读取他们的值。要想读取到值，我们需要访问权限。

``` java
field.setAccessible(true);
```

 完整的：

```java
Employee e = new Employee();
Class cl = e.getClass();
Field field = cl.getDeclaredField("salary");
field.setAccessible(true);
Double sa = (Double)field.get(e);
System.out.println(sa);//1000.0
```

也可以使用getDouble来找出double类型的域

```java
Employee e = new Employee();
Class cl = e.getClass();
Field field = cl.getDeclaredField("salary");
field.setAccessible(true);
Double sa = field.getDouble(e);//IllegalAccessException
System.out.println(sa);
```



设置值的方法：f.set(obj,value)可以将obj对象的f域设置为value

```java
Employee e = new Employee();
Class cl = e.getClass();
Field field = cl.getDeclaredField("salary");
field.setAccessible(true);
field.set(e,1500.0);
Double sa = field.getDouble(e);
System.out.println(sa);//1500.0
```

#### （5）利用反射编写泛型数组代码

```java
String[] strs = {"apple","adad","malad"};
Class cl = strs.getClass();
//cl是class [Ljava.lang.String，[表示一维数组
Class type = cl.getComponentType();//type是class java.lang.String
int length = Array.getLength(strs);
int newLength = length*2;//根据需要newLength也可能小于原数组长度，这里随便取的
Object newArray = Array.newInstance(type,newLength);
System.arraycopy(strs,0,newArray,0,Math.min(length,newLength));
        //参数含义：原数组，从原数组的哪个位置开始拷贝，新数组，从新数组的哪个位置开始拷贝，要拷贝的长度
System.out.println(Arrays.toString((Object[]) newArray));//[apple, adad, malad, null, null, null]
```

==注意如果要把它写成一个方法，strs作为显式参数应该写成Object而不是Object[],因为我们希望什么数组都要扩展，而int[]这类的并不是Object[]类型的：==

```java
Employee e = new Employee();
int[] num={1,2,3};
e.t(num);//t就是扩展数组的方法,这里报错int[]不是Object[]
```

```
public void t(Object[] a){//错误的参数
	//do something
}
```

```
public void t(Object a){//正确的写法
	//do something
}
```

Object可以接受int数组类型

```java
public void t(Object a){
    Class cl = a.getClass();
    Class type = cl.getComponentType();
    int length = Array.getLength(a);
    int newLength = length*2;//根据需要newLength也可能小于原数组长度，这里随便取的
    Object newArray = Array.newInstance(type,newLength);
    System.arraycopy(a,0,newArray,0,Math.min(length,newLength));
    //参数含义：原数组，从原数组的哪个位置开始拷贝，新数组，从新数组的哪个位置开始拷贝，要拷贝的长度
    System.out.println(Arrays.toString((int[])newArray));//[1, 2, 3, 0, 0, 0]
}
```

#### （6）调用任意方法

