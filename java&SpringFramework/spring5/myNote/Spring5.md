# 一.Spring5

## 1.IOC

### 1.1 概念

> 1.IOC（控制反转）：将对象创建和对象与对象之间的调用过程交给spring来管理；



> 2.IOC的底层原理：
>
> xml解析，工厂模式，反射



> 3.为什么要使用IOC？
>
> 降低了耦合度



### 1.2 IOC的xml使用方式：

1.创建一个类：

```java
public class User {
    public void say(){
        System.out.println("hello,spring5");
    }
}
```

2.创建xml文件：

![image-20220402191732622](./Spring5.assets\image-20220402191732622.png)

3.在其中写入：

```xml
<!--  配置javabean对象的路径和名称  -->
<bean id="user" class="com.almond.spring5demo.User"></bean>
```

id人为规定，class为java文件的路径

4.使用时：

```java
//加载对应的spring配置文件
//ClassPathXmlApplicationContext classpath表示路径从src开始
ApplicationContext context = new ClassPathXmlApplicationContext("spring5Demo1.xml");
//通过配置中的id字段获取配置中创建的对象
User user = context.getBean("user", User.class);
System.out.println(user);//com.almond.spring5demo.User@27808f31
user.say(); //hello,spring5
```



### 1.3 IOC的两个接口

2、Spring 提供 IOC 容器实现两种方式：（两个接口） 

（1）BeanFactory：IOC 容器基本实现，是 Spring 内部的使用接口，不提供开发人员进行使用 

* 加载配置文件时候不会创建对象，在获取对象（使用）才去创建对象 

（2）ApplicationContext：BeanFactory 接口的子接口，提供更多更强大的功能，一般由开发人 员进行使用 

* 加载配置文件时候就会把在配置文件对象进行创建

**因为我们需要在服务器启动后快速响应用户，那么这些对象应该在服务器启动前早就创建完成，采用方式2**



### 1.4 依赖注入

DI(依赖注入)：指的是在对象创建后向其中注入属性（实例域）

#### 1.4.1 set方法注入

```xml
<!-- set方法进行依赖注入   -->
<bean id="book" class="com.almond.spring5demo.Book">
    <property name="name" value="java从入门到放弃"></property>
    <property name="author" value="almond"></property>
</bean>
```

Book.java中则必须提供参数的set方法

```java
public class Book {
    private String name;
    private String author;

    public void setName(String name) {
        this.name = name;
    }

    public void setAuthor(String author) {
        this.author = author;
    }
    public void getDetails(){
        System.out.println("name="+name+"author="+author);
    }
}
```

测试类：

```java
@Test
public void testBook1(){
    //set方法实现依赖注入(向创建好的对象注入参数)
    ApplicationContext context = new ClassPathXmlApplicationContext("spring5Demo1.xml");
    Book book = context.getBean("book", Book.class);
    System.out.println(book);//com.almond.spring5demo.Book@1b26f7b2
    book.getDetails();//name=java从入门到放弃author=almond
}
```

#### 1.4.2 构造器注入

**当我们调用xml时，默认使用无参构造器**

使用constructor-arg标签告知使用的是带参数的构造器

```xml
<bean id="order" class="com.almond.spring5demo.Order">
    <constructor-arg name="itemName" value="PC"></constructor-arg>
    <constructor-arg name="location" value="China"></constructor-arg>
</bean>
```

Order.java:必须提供带参数的构造器

```java
public class Order {
    private String location;
    private String itemName;

    public Order(String location, String itemName) {
        this.location = location;
        this.itemName = itemName;
    }
    public void getDetails(){
        System.out.println("location="+location+";itemName="+itemName);
    }
}
```

测试类：

```java
@Test
public void testOrder1(){
    //构造器参数实现依赖注入(向创建好的对象注入参数)
    ApplicationContext context = new ClassPathXmlApplicationContext("spring5Demo1.xml");
    Order order = context.getBean("order", Order.class);
    System.out.println(order);//com.almond.spring5demo.Order@a74868d
    order.getDetails();//location=China;itemName=PC
}
```

#### 1.4.3 特殊的字面量

null:使用标签<null/>放在<property>内

属性中含有<>，使用&lt &gt表示< >

在value标签中 <!CDATA[含有<>的内容]>



#### 1.4.4 注入外部bean

提供两个类BookService，BookDaoImpl

```xml
<bean id="bookService" class="com.almond.service.BookService">
    <!--name为类中属性的名称，这里采用set方法进行依赖注入，类中有set方法的属性才能配置
        ref可以引用其他bean标签-->
    <property name="bookDao" ref="bookDaoImpl"></property>
</bean>

<bean id="bookDaoImpl" class="com.almond.dao.daoImpl.BookDaoImpl"></bean>
```

注意引入外部bean也要创建set方法

```java
public class BookService {
    private BookDao bookDao;

    public void setBookDao(BookDao bookDao) {
        this.bookDao = bookDao;
    }

    public void invokeBookDao(){
        System.out.println("bookService的方法");
        bookDao.getDetails();
    }
}
```



#### 1.4.5 注入内部bean

Emp属性中有Dept对象

```xml
<bean id="emp" class="com.almond.bean.Emp">
    <property name="name" value="almond"></property>
    <property name="gender" value="man"></property>
    <!--dept也要提供set方法-->
    <property name="dept">
        <bean id="dept" class="com.almond.bean.Dept">
            <property name="id" value="1"></property>
            <property name="name" value="财务部"></property>
        </bean>
    </property>
</bean>
```

#### 1.4.6 级联依赖

方式1：直接注入外部bean：

```xml
<bean id="emp" class="com.almond.bean.Emp">
    <property name="name" value="almond"></property>
    <property name="gender" value="man"></property>
    <!--dept也要提供set方法-->
    <property name="dept" ref="dept"></property>
</bean>
<bean id="dept" class="com.almond.bean.Dept">
    <property name="name" value="技术部"></property>
    <property name="id" value="2"></property>
</bean>
```

方式2：在要注入属性的类中写出该属性的get方法

```java
public Dept getDept() {
    return dept;
}
```

在xml中：

```xml
<bean id="emp" class="com.almond.bean.Emp">
    <property name="name" value="almond"></property>
    <property name="gender" value="man"></property>
    <!--在dept有get方法时才能使用如下方式赋值-->
    <property name="dept.name" value="客服"></property>
</bean>
<bean id="dept" class="com.almond.bean.Dept">
    <property name="name" value="技术部"></property>
    <property name="id" value="2"></property>
</bean>
```

#### 1.4.7 注入集合属性

下列操作均在bean标签内：



注入数组

```xml
<property name="course">
    <array>
        <value>java</value>
        <value>c++</value>
    </array>
</property>
```

注入map

```xml
<property name="grade">
    <map>
        <entry key="java" value="80"></entry>
        <entry key="c++" value="70"></entry>
    </map>
</property>
```

注入list

```xml
<property name="name">
    <list>
        <value>杏仁</value>
        <value>Almond</value>
    </list>
</property>
```

注入set

```xml
<property name="sets">
    <set>
        <value>mysql</value>
        <value>oracle</value>
    </set>
</property>
```



集合属性中有对象：

```xml
<bean id="student" class="com.almond.collections.Student">
    
    <property name="courseList">
        <list>
            <ref bean="course1"></ref>
            <ref bean="course2"></ref>
        </list>
    </property>

</bean>

<bean id="course1" class="com.almond.collections.Course">
    <property name="cname" value="c++"></property>
</bean>
<bean id="course2" class="com.almond.collections.Course">
    <property name="cname" value="python"></property>
</bean>
```



#### 1.4.8 抽取集合

在xml文件开头修改，增加一个xmlns:util，修改xsi:schemaLocation 对照已有的url，将bean或者beans的位置改为util

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:util="http://www.springframework.org/schema/util"
       xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd
                           http://www.springframework.org/schema/util http://www.springframework.org/schema/util/spring-util.xsd">
```

在xml中写入：

```xml
<!--抽取list，需要先增加和修改beans标签里的参数xmlns:util和xsi:schemaLocation-->
<util:list id="language1">
    <value>c</value>
    <value>c++</value>
    <value>java</value>
    <value>go</value>
</util:list>

<bean id="languages" class="com.almond.collections.Language">
    <property name="lang" ref="language1"></property>
</bean>
```

#### 1.4.9 工厂bean

实现FactoryBean<>接口的类可以作为工厂bean，其中实现getObject方法时返回新的实例

```java
public class MyBean implements FactoryBean<Course> {
    //定义返回的bean对象
    @Override
    public Course getObject() throws Exception {
        Course course = new Course();
        course.setCname("java");
        return course;
    }

    @Override
    public Class<?> getObjectType() {
        return null;
    }

    //是否是单例模式
    @Override
    public boolean isSingleton() {
        return false;
    }
}
```

xml配置方法：

```xml
<bean id="mybean" class="day2.almond.MyBean"></bean>
```

### 1.5 bean的管理

#### 1.5.1 默认获取单例对象

在Spring中，默认情况下，bean为单例对象

```xml
<bean id="user" class="com.almond.spring5demo.User"></bean>
```

测试无论获取多少次，获取到的都是一个对象：

```java
//xml配置默认情况下是单例
@Test
public void testMyBean2(){
    ApplicationContext context = new ClassPathXmlApplicationContext("spring5Demo1.xml");
    //通过配置中的id字段获取配置中创建的对象
    User user1 = context.getBean("user", User.class);
    User user2 = context.getBean("user", User.class);
    System.out.println(user1==user2);//true
}
```

#### 1.5.2  设置获取多例对象

设置scope属性为protype即可，默认为singleton（单例）

```xml
<bean id="book" class="com.almond.spring5demo.Book" scope="prototype"></bean>
```

#### 1.5.3 bean的生命周期

（1）通过构造器创建 bean 实例（无参数构造） 

（2）为 bean 的属性设置值和对其他 bean 引用（调用 set 方法） 

（3）调用 bean 的初始化的方法（需要进行配置初始化的方法） 

（4）bean 可以使用了（对象获取到了） 

（5）当容器关闭时候，调用 bean 的销毁的方法（需要进行配置销毁的方法）



在xml中配置init-method，destroy-method两个参数为初始化方法和销毁方法

```xml
<bean id="lifeCycle" class="day2.almond.LifeCycle" init-method="initMethod" destroy-method="destoryMethod">
    <property name="value" value="v1"></property>
</bean>
```



在配置的类中添加初始化（initMethod）和销毁方法（destoryMethod）

```java
public class LifeCycle {
    private String value;

    public LifeCycle() {
        System.out.println("构造器被调用");
    }

    public void setValue(String value) {
        this.value = value;
        System.out.println("set方法被调用");
    }
    public void initMethod(){
        System.out.println("初始化方法被调用");
    }
    public void destoryMethod(){
        System.out.println("销毁方法被调用");
    }
    
}
```

测试：

```java
//生命周期测试
@Test
public void testLifeCycle(){
    ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext("day2FactoryBean.xml");
    LifeCycle lifeCycle = context.getBean("lifeCycle", LifeCycle.class);
    System.out.println("实例对象被获取");
    //手动销毁bean对象
    context.close();

    //构造器被调用
    //set方法被调用
    //初始化方法被调用
    //实例对象被获取
    //销毁方法被调用
}
```



#### 1.5.4 增加后置处理器的生命周期

（1）通过构造器创建 bean 实例（无参数构造） 

（2）为 bean 的属性设置值和对其他 bean 引用（调用 set 方法） 

（3）把 bean 实例传递 bean 后置处理器的方法 postProcessBeforeInitialization  

（4）调用 bean 的初始化的方法（需要进行配置初始化的方法） 

（5）把 bean 实例传递 bean 后置处理器的方法 postProcessAfterInitialization 

（6）bean 可以使用了（对象获取到了） 

（7）当容器关闭时候，调用 bean 的销毁的方法（需要进行配置销毁的方法）



在一个xml文件中配置后置处理器后，这个xml文件中所有bean都加上了后置处理器

使用方法：

我们需要一个实现了BeanPostProcessor接口的类来作为后置处理器：

```java
public class LifeCycle2 implements BeanPostProcessor {
    private String name="l2";

    @Override
    public Object postProcessBeforeInitialization(Object bean, String beanName) throws BeansException {
        System.out.println(beanName+":"+"在初始化之前执行的后置处理器方法");
        return bean;
    }

    @Override
    public Object postProcessAfterInitialization(Object bean, String beanName) throws BeansException {
        System.out.println(beanName+":"+"在初始化之后执行的后置处理器方法");
        return bean;
    }

}
```

在xml文件中配置这个后置处理器

```xml
<!--配置后置处理器，注意后置处理器会将整个xml文件中的所有bean都加上后置处理器    -->
<bean id="lifeCycle2" class="day2.almond.LifeCycle2">
</bean>
```

测试：

```java
//生命周期测试加入前置处理器后置处理器
@Test
public void testLifeCycle2(){
    ClassPathXmlApplicationContext context = new ClassPathXmlApplicationContext("day2FactoryBean.xml");
    LifeCycle lifeCycle = context.getBean("lifeCycle", LifeCycle.class);
    System.out.println("实例对象被获取");
    //手动销毁bean对象
    context.close();
    //构造器被调用
    //set方法被调用
    //lifeCycle:在初始化之前执行的后置处理器方法
    //初始化方法被调用
    //lifeCycle:在初始化之后执行的后置处理器方法
    //实例对象被获取
    //销毁方法被调用
}
```

**如果一个xml中有多个bean，后置处理器的同一个方法会被调用多次**

#### 1.5.5 自动装配

根据指定装配规则（属性名称或者属性类型），Spring 自动将匹配的属性值进行注入



方式1： byName，按照id来匹配

```xml
<!--自动装配，
byName：要求要引入的bean的id值和emp中的属性值相同
byType：按照类名进行匹配，如果有多个bean且class都和属性匹配则会报错-->
<bean id="emp" class="com.almond.bean.Emp" autowire="byName"></bean>
<bean id="dept" class="com.almond.bean.Dept"></bean>
```



方式2：直接根据类型确定对象

```xml
 <!--自动装配，
    byName：要求要引入的bean的id值和emp中的属性值相同
    byType：按照类名进行匹配，如果有多个bean且class都和属性匹配则会报错-->
    <bean id="emp" class="com.almond.bean.Emp" autowire="byType"></bean>
    <bean class="com.almond.bean.Dept"></bean>
```

#### 1.5.6 xml中导入外部属性文件

当我们要配置druid连接池时，可以采取如下操作



建立一个dataSource.properties文件作为外部文件:

```properties
p.driverClass=com.mysql.jdbc.Driver
p.url=jdbc:mysql://localhost:3306/test
p.username=root
p.password=932723
```

在xml中：

1.配置beans标签，

​	复制xmlns的url新建一个xmlns:context

​	将xmlns:context的url换为新建的xmlns:后面的单词(这里为context)

​	增加xsi:schemaLocation 的url 相较于前两个url，只是将bean或beans改为context

2.引入配置文件

3.使用${}语法引入配置文件的属性

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       http://www.springframework.org/schema/context
       http://www.springframework.org/schema/context/spring-context.xsd">
    
    <!--引入配置文件-->
    <context:property-placeholder location="dataSource.properties"/>
    
    <bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource">
        <!--        这里的${}是spring中的一种语法        -->
        <property name="driverClassName" value="${p.driverClass}"></property>
        <property name="url" value="${p.url}"></property>
        <property name="username" value="${p.username}"></property>
        <property name="password" value="${p.password}"></property>
    </bean>
</beans>
```

### 1.6 注解方式操作bean

#### 1.6.1 什么是注解 

（1）注解是代码特殊标记，

​			格式：@注解名称(属性名称=属性值, 属性名称=属性值..) 

（2）使用注解，注解作用在类上面，方法上面，属性上面 

（3）使用注解目的：简化 xml 配置

#### 1.6.2 bean中的注解

（1）@Component 	普通pojo实例化到spring容器

（2）@Service	用于service层，注入dao						

（3）@Controller 	web层，注入service

（4）@Repository 	用于dao层，实现dao访问

上面四个注解功能是一样的，为了区分不同层才创建不同的注解

#### 1.6.3 基于注解方式实现对象的创建

1.引入Spring-aop-5.2.6.RELEASE.jar

2.开启组件扫描

配置beans标签：

```xml
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/context
       http://www.springframework.org/schema/context/spring-context.xsd">
```

开启组件扫描：

```xml
<!--    组件扫描    -->
<!--    要扫描多个包，不同包之间用逗号隔开；或者扫描不同包的上层目录   -->
<context:component-scan base-package="com.almond"></context:component-scan>
```

3.在类上加注解

```java
//value相当于xml方式中的bean标签中的id
// 这里相当于<bean id="person01">,如果不写value默认为类名第一个字母小写
@Component(value="person01")
public class Person1 {...}
```

#### 1.6.4 xml中filter的设置

use-default-filters默认为true

仅扫描@Component注解：

```xml
<!--            不使用默认filters可以设置扫描的注解名     -->
<context:component-scan base-package="com.almond" use-default-filters="false">
    <context:include-filter type="annotation"
 expression="org.springframework.stereotype.Component"/>
</context:component-scan>
```



不扫描@Component注解：

```xml
<!--            使用默认filters可以设置不扫描的注解名-->
<context:component-scan base-package="com.almond">
    <context:exclude-filter type="annotation"            expression="org.springframework.stereotype.Component"/>
</context:component-scan>
```



#### 1.6.5 基于注释的属性注入

PersonDaoImpl类已经提供了注解：

```java
@Repository(value = "personDaoImpl1")
public class PersonDaoImpl implements PersonDao{
    @Override
    public void outPut() {
        System.out.println("personDaoImpl的outPut方法");
    }
}
```



（1）@Autowired：根据类型自动装配合适的对象
在需要注入的属性上添加注解，而且不需要提供set方法，因为内部已经封装过

```java
@Autowired
private PersonDao personDao;
```



不仅set方法，get方法也被封装了，可以直接调用

```java
   @Test
    public void testPerson12(){
        ClassPathXmlApplicationContext cpxac = new ClassPathXmlApplicationContext("Bean1.xml");
        Person1 p1 = cpxac.getBean("person01", Person1.class);
        p1.outPut();//Person1,注解创建的对象
//      get方法也被封装可以直接调用private属性
        p1.personDao.outPut();//personDaoImpl的outPut方法
    }
```



（2）@Qualifier：根据名称进行注入

这个@Qualifier 注解的使用，和上面@Autowired 一起使用

```java
@Autowired
@Qualifier(value = "personDaoImpl1") //根据名称注入，如果有多个子类继承同一个接口，根据类型无法判断要注入哪一个，这时需要名称注入
private PersonDao personDao;

```



（3）@Resource：可以根据类型注入，可以根据名称注入

```java
//根据名称注入，注意要写的参数是name，不过也是根据要调用的类注解中的value值判断的
@Resource(name = "personDaoImpl1")
private PersonDao personDao;
```

---

```java
//也可以直接进行类型注入
@Resource
private PersonDao personDao;
```



（4）@Value：注入普通类型属性

```java
//value可以设置基本类型变量的值，这里是写死的，真正使用时是利用${}到xml中寻找值的
@Value(value="abcd")
private String name;
```

#### 1.6.6 完全注解开发

创建一个类做如下配置：

```java
@Configuration//表示为配置类
@ComponentScan(basePackages = {"com.almond"})//设置扫描范围
public class SpringConfig {

}
```

这个类相当于xml文件

```java
//配置类方法
@Test
public void testPerson14(){
    ApplicationContext context = new AnnotationConfigApplicationContext(SpringConfig.class);
    
    Person1 p1 = context.getBean("person01", Person1.class);
    System.out.println(p1.name);//abcd
}
```



## 2.AOP

### 2.1 概念以及理解

面向切面编程，利用 AOP 可以对业务逻辑的各个部分进行隔离，从而使得业务逻辑各部分之间的耦合度降低，提高程序的可重用性，同时提高了开发的效率。



理解：不需要修改原来的代码，通过在主功能里增加新的功能，就可以实现功能的增加



举例说明：

如果我们已经实现了一个登录的功能，现在突然有了新的需求，我们想要根据登录的用户不同设置不同的权限，如何解决？

按照传统思路，我们会在登录的逻辑判断处中加入一段代码判断用户的权限；

面向切面的思路：我们写出权限判断的逻辑，将这段逻辑嵌入到主逻辑判断的位置；

### 2.2 底层实现

需要使用动态绑定的方式实现，需要增强的类为被代理类

有两种情况：

被代理类有接口，JDK完成动态代理

被代理类没有接口，CGLIB完成动态代理

#### 2.1 JDK实现动态代理

例如：

我们目前有接口

```java
public interface UserDao {
    public int add (int x,int y);
    public void update();
}
```

接口的实现类：

```java
public class UserDaoImpl implements UserDao {

    @Override
    public int add(int x, int y) {
        System.out.println("原始的add方法");
        return x+y;
    }

    @Override
    public void update() {
        System.out.println("原始update方法");
    }
}
```

要增强实现类的功能，调用java.lang.reflect.Proxy中的newInstance方法

参数说明：

一个类加载器，使用当前类即可（什么类都可以） 

被代理对象实现的接口数组：可以调用被代理类的getInterfaces方法（这里就是userDao.class，直接写出来了）

实现了InvocationHandler的代理类：后面讨论

```java
public static void main(String[] args) {
    Class[] interfaces = {UserDao.class};
    //参数 一个类加载器 被代理对象的接口 实现了InvocationHandler的代理类
    //返回值类型为 代理对象 代理对象是实现了被代理接口的实现类，是UserDao但不是UserDaoImpl
    UserDao dao = (UserDao) Proxy.newProxyInstance(UserProxy.class.getClassLoader(), interfaces,
            new UserProxyHandler(new UserDaoImpl()));

    int add = dao.add(1, 2);
    System.out.println(add);
    dao.update();
	//在add之前做一些操作
    //原始的add方法
    //在add之后做一些操作
    //3
    
    //在update前做一些操作
    //原始update方法
    //在update后做一些操作
}
```



实现了InvocationHandler的代理类：

```java
class UserProxyHandler implements InvocationHandler {

    //因为我们要增强被代理的对象的方法，首先需要有这个被代理的对象，通过在代理类中设置实例域并提供有参数的构造器得到
    private UserDaoImpl userDao;

    public UserProxyHandler(Object userDao) {
        this.userDao = (UserDaoImpl) userDao;
    }

    //当UserProxyImpl被创建时自动调用invoke方法
    //method为  我们通过newInstance方法获取到代理对象，调用这个代理对象的新方法，这个新方法就是method
    //args为 这个新方法的参数
    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {
        if (method.getName() == "add") {
            System.out.println("在add之前做一些操作");
            //第一个参数是被代理对象，用来调用原始方法，第二个参数是被调用方法的参数
            Object res = method.invoke(userDao, args);
            System.out.println("在add之后做一些操作");
            return res;//注意一定要返回原方法的结果
        }
        if (method.getName() == "update") {
            System.out.println("在update前做一些操作");
            method.invoke(userDao, args);
            System.out.println("在update后做一些操作");
            //原方法为void不需要返回
        }
        return null;
    }
}
```

### 2.3 AOP术语

1.连接点：类里可以被增强的方法（实际上可能没有被增强），称为连接点

2.切入点：实际被增强的方法称为切入点

3.通知（增强）：实际增强的逻辑部分称为通知/增强

​	通知分为

​		前置通知@Before

​		后置通知@AfterReturning

​		环绕通知@Around

​		异常通知@AfterThrowing

​		最终通知@After

4.切面：将通知应用到切入点的过程

### 2.4 AOP操作

AspectJ 不是 Spring 组成部分，是独立 AOP 框架，一般把 AspectJ 和 Spirng 框架一起使 用，进行 AOP 操作

方式：注解方式实现（也可以xml配置，不常用）

#### 2.4.1 前置准备

1.加入依赖

![image-20220406200852817](./Spring5.assets\image-20220406200852817.png)

2.切入点表达式

```
execution( 权限修饰符 返回类型 类全路径.方法名称(参数列表) )

举例 1：对 com.atguigu.dao.BookDao 类里面的 add 进行增强
execution(* com.atguigu.dao.BookDao.add(..))
举例 2：对 com.atguigu.dao.BookDao 类里面的所有的方法进行增强
execution(* com.atguigu.dao.BookDao.* (..))
```

#### 2.4.2 操作

1.创建被代理类

```java
@Component//注解方式创建bean对象，保证被注解扫描搜索到
public class Person {
    public void add(){
        //待增强的方法
        System.out.println("Person的add方法");
    }
}
```

2.创建代理类

```java
@Component//代理类也要被注解扫描搜索到
@Aspect//代理类的标识,生成代理类的对象
public class PersonProxy {
    //添加注解，设置value字段为切入表达式
	@Before(value = "execution(* com.almond.aspectanno.Person.add(..))")
    public void before() {
        System.out.println("before方法被调用");
    }
}
```

3.通知配置

配置了AOP和context

```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:aop="http://www.springframework.org/schema/aop"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       http://www.springframework.org/schema/context
       http://www.springframework.org/schema/context/spring-context.xsd http://www.springframework.org/schema/aop https://www.springframework.org/schema/aop/spring-aop.xsd">
<!--    开启注解扫描-->
    <context:component-scan base-package="com.almond.aspectanno"></context:component-scan>

<!--    开启Aspectj生成代理对象,具体作用是在扫描范围中查找是否有@Aspect注解的类，如果有则将这个类生成为代理对象-->
    <aop:aspectj-autoproxy></aop:aspectj-autoproxy>
</beans>
```

 

#### 2.4.3 各个注解的表示方法

各个注解的表示方法：

```java
@Component
@Aspect
public class PersonProxy {
    @Before(value = "execution(* com.almond.aspectanno.Person.add(..))")
    public void before() {
        System.out.println("before方法被调用");
    }

    @After(value = "execution(* com.almond.aspectanno.Person.add(..))")
    public void after() {
        System.out.println("after方法被调用");
    }

    @AfterReturning(value = "execution(* com.almond.aspectanno.Person.add(..))")
    public void afterReturning() {
        System.out.println("afterReturning方法被调用");
    }

    @AfterThrowing(value = "execution(* com.almond.aspectanno.Person.add(..))")
    public void afterThrowing() {
        System.out.println("afterThrowing方法被调用");
    }

    @Around(value = "execution(* com.almond.aspectanno.Person.add(..))")
    public void around(ProceedingJoinPoint p) throws Throwable {
        System.out.println("原方法前around方法被调用");
        p.proceed();//执行被代理类的原方法
        System.out.println("原方法后around方法被调用");
    }

}
```

结果在没有异常时:

```java
//原方法前around方法被调用
//before方法被调用
//Person的add方法
//原方法后around方法被调用
//after方法被调用
//afterReturning方法被调用
```

在出现异常时：

``` java
//原方法前around方法被调用
//before方法被调用
//after方法被调用 after相当于finally，不论有无异常都会被执行
//afterThrowing方法被调用 异常处理
//报错
```



#### 2.4.4 抽取注解

如果有很多增强加在同一个切入点，可以将这个切入点路径抽取出来，安置在一个空方法上

```java
@Pointcut(value = "execution(* com.almond.aspectanno.Person.add(..))")
public void point(){
}
```

其他方法都可以直接调用该方法拿到切入点：

```java
@Before(value="point()")
public void before() {
    System.out.println("before方法被调用");
}

@After(value="point()")
public void after() {
    System.out.println("after方法被调用");
}

@AfterReturning(value="point()")
public void afterReturning() {
    System.out.println("afterReturning方法被调用");
}

@AfterThrowing(value="point()")
public void afterThrowing() {
    System.out.println("afterThrowing方法被调用");
}

@Around(value="point()")
public void around(ProceedingJoinPoint p) throws Throwable {
    System.out.println("原方法前around方法被调用");
    p.proceed();//执行被代理类的原方法
    System.out.println("原方法后around方法被调用");
}
```



#### 2.4.5 设置同一切面的多个注解的优先级

设置@Order注解，数字越小优先级越高

```java
@Before(value="point()")
@Order(1)
public void before() {
    System.out.println("before方法被调用");
}
```



#### 2.4.6 完全注解开发

``` java
@Configuration 
@ComponentScan(basePackages = {"com.almond"}) @EnableAspectJAutoProxy(proxyTargetClass = true) 
public class ConfigAop{ 
}
```



## 3.jdbcTemplate

### 3.1 前置准备

1.导包

![image-20220406203309646](./Spring5.assets\image-20220406203309646.png)

2.配置druid数据库连接池（前面讲过，注意修改一下url）

```xml
<!--引入配置文件-->
<context:property-placeholder location="dataSource.properties"/>
<bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource">
    <!--        这里的${}是spring中的一种语法        -->
    <property name="driverClassName" value="${p.driverClass}"></property>
    <property name="url" value="${p.url}"></property>
    <property name="username" value="${p.username}"></property>
    <property name="password" value="${p.password}"></property>
</bean>
```

3.配置jdbcTemplate,在参数中引入数据库连接池

```xml
<!--    创建jdbcTemplate对象，并向其中注入连接池-->
    <bean id="jdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
<!--         这个类中用构造器调用了父类中的set方法所以仍然采用set注入-->
        <property name="dataSource" ref="dataSource"></property>
    </bean>
```

4.开启组件扫描

```xml
<!--    组件扫描-->
    <context:component-scan base-package="com.almond"></context:component-scan>
```

5.创建dao对象，向其中注入jdbcTemplate

```java
@Repository
public class BookDaoImpl implements BookDao {
    @Autowired
    private JdbcTemplate jdbcTemplate;
	...
}
```

6.创建service对象，向其中注入dao

```java
@Service
public class BookService {
    @Autowired
    private BookDao bookDao;
	...
}
```

下面就可以操作数据库了

### 3.2 操作数据库

service：

```java
@Service
public class BookService {
    @Autowired
    private BookDao bookDao;

    /**
     * 插入book数据到数据库
     * @param book
     */
    public void addBook(Book book){
        bookDao.add(book);
    }

    /**
     * 通过id修改book
     * @param book
     */
    public void updateBook(Book book){
        bookDao.update(book);
    }

    /**
     * 通过id删除book
     * @param id
     */
    public void deleteBook(Integer id){
        bookDao.delete(id);
    }

    /**
     * 查找book的记录数
     * @return
     */
    public Integer findBookCount(){
       return bookDao.findCount();
    }

    /**
     * 查找指定id的book
     * @param id
     * @return
     */
    public Book findBook(Integer id){
       return bookDao.findBook(id);
    }

    /**
     * 查找所有的book
     * @return
     */
    public List<Book> findAllBook(){
       return bookDao.findAllBook();
    }

    /**
     * 批量增加book
     * @param argsList list的每个元素都是一个Object[]对应一行数据，Object[]中的一个元素对应一个字段
     */
    public void addBookList(List<Object[]> argsList){
        bookDao.addBookList(argsList);
    }

    /**
     * 批量修改book
     * @param argsList list的每个元素都是一个Object[]对应一行数据，Object[]中的一个元素对应一个字段
     */
    public void updateBookList(List<Object[]> argsList){
        bookDao.updateBookList(argsList);
    }

    /**
     * 批量删除book
     * @param argsList list的每个元素都是一个Object[]对应一行数据，Object[]中的一个元素对应一个字段
     */
    public void deleteBookList(List<Object[]> argsList){
        bookDao.deleteBookList(argsList);
    }

}
```

dao接口：

```java
public interface BookDao {
    /**
     * 增加book
     * @param book
     */
    void add(Book book);

    /**
     * 提供一个book对象按照其id进行修改
     * @param book
     */
    void update(Book book);

    /**
     * 按照id进行删除
     * @param id
     */
    void delete(Integer id);

    /**
     * 返回book的记录数
     * @return
     */
    Integer findCount();

    /**
     * 根据id查询
     * @param id
     * @return
     */
    Book findBook(Integer id);

    /**
     * 查询整张表
     * @return
     */
    List<Book> findAllBook();

    /**
     * 批量添加操作
     * @param argsList
     */
    void addBookList(List<Object[]> argsList);
    /**
     * 批量修改操作
     * @param argsList
     */
    void updateBookList(List<Object[]> argsList);
    /**
     * 批量删除操作
     * @param argsList
     */
    void deleteBookList(List<Object[]> argsList);
}
```

dao实现类：

注意查询操作参数中要声明一个BeanPropertyRowMapper实例

```java
@Repository
public class BookDaoImpl implements BookDao {
    @Autowired
    private JdbcTemplate jdbcTemplate;


    @Override
    public void add(Book book) {
        String sql = "insert into `t_book`(id,name,status) values(?,?,?)";
        Object[] args = {book.getId(),book.getName(),book.getStatus()};
        jdbcTemplate.update(sql,args);
    }

    @Override
    public void update(Book book) {
        String sql ="update `t_book` set `name`=?,`status`=? where id =?";
        Object[] args ={book.getName(),book.getStatus(),book.getId()};
        int update = jdbcTemplate.update(sql, args);
        System.out.println("影响的行数:"+update);
    }

    @Override
    public void delete(Integer id) {
        String sql = "delete from `t_book` where id = ?";
        jdbcTemplate.update(sql,id);
    }

    @Override
    public Integer findCount() {
        String sql ="select count(*) from `t_book`";
        return jdbcTemplate.queryForObject(sql,Integer.class);
    }

    @Override
    public Book findBook(Integer id) {
        String sql = "select `id`,`name`,`status` from t_book where id =?";
        return jdbcTemplate.queryForObject(sql,new BeanPropertyRowMapper<Book>(Book.class),id);
    }

    @Override
    public List<Book> findAllBook() {
        String sql = "select `id`,`name`,`status` from t_book";
        return jdbcTemplate.query(sql,new BeanPropertyRowMapper<Book>(Book.class));
    }

    @Override
    public void addBookList(List<Object[]> argsList) {
        String sql = "insert into `t_book`(id,name,status) values(?,?,?)";
        //batchUpdate会取出List里的每一个Object数组，根据数组中的Object给每个字段赋值，一个Object数组相当于一条记录
        int[] ans = jdbcTemplate.batchUpdate(sql, argsList);
        System.out.println("分别改变了"+ Arrays.toString(ans) +"条记录");
    }

    @Override
    public void updateBookList(List<Object[]> argsList) {
        String sql ="update `t_book` set `name`=?,`status`=? where id =?";
        int[] ans = jdbcTemplate.batchUpdate(sql, argsList);
        System.out.println("分别改变了"+ Arrays.toString(ans) +"条记录");
    }

    @Override
    public void deleteBookList(List<Object[]> argsList) {
        String sql = "delete from `t_book` where id = ?";
        jdbcTemplate.batchUpdate(sql,argsList);
    }
}
```

测试类：

加载xml文件，从context中拿取service实例进行测试

```java
public class BookServiceTest {

    @Test
    public void add() {
        ApplicationContext context = new ClassPathXmlApplicationContext("Bean1.xml");
        Book book = new Book(null, "java核心技术", "状态良好");
        BookService bookService = context.getBean("bookService", BookService.class);
        bookService.addBook(book);
    }
    @Test
    public void update() {
        ApplicationContext context = new ClassPathXmlApplicationContext("Bean1.xml");
        Book book = new Book(1, "java从入门到放弃", "崭新出厂");
        BookService bookService = context.getBean("bookService", BookService.class);
        bookService.updateBook(book);
    }
    @Test
    public void delete() {
        ApplicationContext context = new ClassPathXmlApplicationContext("Bean1.xml");
        BookService bookService = context.getBean("bookService", BookService.class);
        bookService.deleteBook(1);
    }
    @Test
    public void findCount() {
        ApplicationContext context = new ClassPathXmlApplicationContext("Bean1.xml");
        BookService bookService = context.getBean("bookService", BookService.class);
        Integer bookCount = bookService.findBookCount();
        System.out.println(bookCount);
    }
    @Test
    public void findBook() {
        ApplicationContext context = new ClassPathXmlApplicationContext("Bean1.xml");
        BookService bookService = context.getBean("bookService", BookService.class);
        Book book = bookService.findBook(2);
        System.out.println(book);
    }
    @Test
    public void findAllBook() {
        ApplicationContext context = new ClassPathXmlApplicationContext("Bean1.xml");
        BookService bookService = context.getBean("bookService", BookService.class);
        List<Book> list = bookService.findAllBook();
        System.out.println(list);
    }

    @Test
    public void addBookList() {
        ArrayList<Object[]> args = new ArrayList<>();
        Object[] o1 = {null,"c++程序设计","崭新出厂"};
        Object[] o2 = {null,"java核心技术卷二","久经沙场"};
        Object[] o3 = {null,"mysql","战痕累累"};
        args.add(o1);
        args.add(o2);
        args.add(o3);

        ApplicationContext context = new ClassPathXmlApplicationContext("Bean1.xml");
        BookService bookService = context.getBean("bookService", BookService.class);
        bookService.addBookList(args);
    }

    @Test
    public void updateBookList() {
        ArrayList<Object[]> args = new ArrayList<>();
        Object[] o1 = {"c++程序设计01","崭新出厂",7};
        Object[] o2 = {"java核心技术卷二01","久经沙场1",8};
        Object[] o3 = {"mysql01","战痕累累1",9};
        args.add(o1);
        args.add(o2);
        args.add(o3);

        ApplicationContext context = new ClassPathXmlApplicationContext("Bean1.xml");
        BookService bookService = context.getBean("bookService", BookService.class);
        bookService.updateBookList(args);
    }
    @Test
    public void deleteBookList() {
        ArrayList<Object[]> args = new ArrayList<>();
        Object[] o1 = {7};
        Object[] o2 = {8};
        Object[] o3 = {9};
        args.add(o1);
        args.add(o2);
        args.add(o3);

        ApplicationContext context = new ClassPathXmlApplicationContext("Bean1.xml");
        BookService bookService = context.getBean("bookService", BookService.class);
        bookService.deleteBookList(args);
    }

}
```

### 3.3 数据库事务介绍

1.事务添加到 JavaEE 三层结构里面 Service 层（业务逻辑层）

2、在 Spring 进行事务管理操作 

（1）有两种方式：编程式事务管理（手动在service层的代码上加try-catch）和声明式事务管理（使用） 

3、声明式事务管理 

（1）基于注解方式（使用） （2）基于 xml 配置文件方式

4、在 Spring 进行声明式事务管理，底层使用 AOP 原理

### 3.4 数据库事务

例子:a给b转账

1.在xml中配置事务：（已经完成了数据库连接池，组件扫描，jdbcTemplate的配置）

（1）配置事务管理器：

```xml
<!--    配置事务管理器，向事务管理器中注入数据库连接池-->
    <bean id="dataSourceTransactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
        <property name="dataSource" ref="dataSource"></property>
    </bean>
```

（2）配置beans标签：

```xml
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:tx="http://www.springframework.org/schema/tx"
       xmlns:aop="http://www.springframework.org/schema/aop"
       xsi:schemaLocation="http://www.springframework.org/schema/beans
       http://www.springframework.org/schema/beans/spring-beans.xsd
       http://www.springframework.org/schema/context
       http://www.springframework.org/schema/context/spring-context.xsd
       http://www.springframework.org/schema/aop
       https://www.springframework.org/schema/aop/spring-aop.xsd
       http://www.springframework.org/schema/tx
       https://www.springframework.org/schema/tx/spring-tx.xsd">
    ...
```

（3）开启事务：

``` xml
<!--    开启事务-->
    <tx:annotation-driven transaction-manager="dataSourceTransactionManager"></tx:annotation-driven>
```

2.配置dao接口

```java
public interface UserDao {
    /**
     * 给指定id的用户增加钱
     * @param id
     * @param money
     */
    public void addMoney(int id,int money);

    /**
     * 给指定id的用户减少钱
     * @param id
     * @param money
     */
    public void reduceMoney(int id ,int money);
}
```

3.配置daoImpl：

加上注释@Repository，注入JdbcTemplate，实现方法

```java
@Repository
public class UserDaoImpl implements UserDao {
    @Autowired
    private JdbcTemplate jdbcTemplate;

    @Override
    public void addMoney(int id, int money) {
        String sql ="update `t_account` set money=money+? where id = ?";
        Object[] args ={money,id};
        jdbcTemplate.update(sql,args);
    }

    @Override
    public void reduceMoney(int id, int money) {
        String sql ="update `t_account` set money=money-? where id = ?";
//        int i = 1/0;
        Object[] args ={money,id};
        jdbcTemplate.update(sql,args);
    }
}
```

4.配置service：

（1）加上注释@Service

（2）加上注释@Transactional

类上加@Transactional注解表示整个类的所有方法都引入事务；

@Transactional可以用在单个方法前，表示仅有这个方法实现事务。

（3）注入UserDao

```java
@Service
@Transactional
public class UserService{
    @Autowired
    private UserDao userDao;


    public void transferAccount(int reduceId,int addId,int amount){
        userDao.reduceMoney(reduceId,amount);
        userDao.addMoney(addId,amount);
    }
}
```

测试类：

```java
@Test
public void testMoney(){
    ApplicationContext context = new ClassPathXmlApplicationContext("Bean1.xml");
    UserService userService = context.getBean("userService", UserService.class);
    userService.transferAccount(1,2,100);
}
```

### 3.5 @Transactional参数

#### 3.5.1 使用格式

```
@Transactional(propagation = Propagation.REQUIRED)
```

#### 3.5.2 propagation事务传播行为

对一个带事务注解的方法内调用了另一个不带注解的操作数据库方法这种情况进行管理。有七种属性。

常用的：

REQUIRED： 

有事务就在当前事务中进行，没有事务还调用别的事务方法就创建新事务。情况如下：

​	a方法有事务，调用没有事务的b方法，b方法使用a的当前事务

​	a方法没有事务，调用无事务的b方法，创建新事务



REQUIRED_NEW：

当前调用的新方法必须要有事务运行，例如：

a方法调用b方法，不论a有无事务都创建新的事务

#### 3.5.3 ioslation事务隔离级别

格式：

```java
@Transactional(isolation = Isolation.READ_COMMITTED)
```

#### 3.5.4 timeout超时时间

（1）事务需要在一定时间内进行提交，如果不提交进行回滚 

（2）默认值是 -1 ，设置时间以秒单位进行计算

#### 3.5.5 readOnly是否只读

（1）读：查询操作，写：添加修改删除操作 

（2）readOnly 默认值 false，表示可以查询，可以添加修改删除操作 

（3）设置 readOnly 值是 true，设置成 true 之后，只能查询

#### 3.5.6 rollbackFor：回滚

设置出现哪些异常进行事务回滚

#### 3.5.7 noRollbackFor：不回滚

设置出现哪些异常不进行事务回滚

### 3.6 XML声明式事务管理

1.配置事务管理器：

```xml
<!--    1.配置事务管理器-->
<bean id="transactionManager" class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
    <property name="dataSource" ref="dataSource"></property>
</bean>
```

2.配置通知

```xml
<!--    2.配置通知-->
<tx:advice id="txAdvice">
    <tx:attributes>
        <!--            配置在那个方法上加上事务以及事务的参数-->
        <tx:method name="transferAccount" propagation="REQUIRED"/>
    </tx:attributes>
</tx:advice>
```

3.配置切入点和切面

```xml
<!--    配置切入点和切面-->
<aop:config>
    <!--        切入点为UserService中的所有方法-->
    <aop:pointcut id="pointcut" expression="execution(* com.almond.service.UserService.*(..))"/>
    <!--        配置切面,把通知给切入点-->
    <aop:advisor advice-ref="txAdvice" pointcut-ref="pointcut"></aop:advisor>
</aop:config>
```

### 3.7 完全注解式声明事务管理

创建配置类BeanConfig，

声明方法返回为我们要创建的<bean>中的对象，在方法上加@Bean表示在IOC容器中存储这个对象

```java
@Configuration//说明本类是配置类
@ComponentScan(basePackages = "com.almond")//组件扫描
@EnableTransactionManagement//开启事务
public class BeanConfig {

    //创建数据库连接池
    @Bean//表示在IOC创建对象
    public DataSource getDataSource() throws IOException {
        //读取数据库配置文件
        Properties properties = new Properties();
        //注意这里路径包括src，如果不包括src虽然ctrl点击可以跳转到指定文件，但是运行时找不到这个文件
        InputStream inputStream = new BufferedInputStream(new FileInputStream("src/dataSource.properties"));
        properties.load(inputStream);
        String driverClass = (String) properties.get("p.driverClass");
        String url = (String) properties.get("p.url");
        String username = (String) properties.get("p.username");
        String password = (String) properties.get("p.password");
        //新建数据库连接池，并设置属性
        DruidDataSource dataSource = new DruidDataSource();
        dataSource.setDriverClassName(driverClass);
        dataSource.setUrl(url);
        dataSource.setUsername(username);
        dataSource.setPassword(password);
        return dataSource;
    }

    //创建jdbcTemplate对象,向其中注入我们创建好的数据库连接池
    @Bean//表示在IOC创建对象
    public JdbcTemplate getJdbcTemplate(DataSource dataSource){//参数表示从IOC容器中取出我们已经创建好的数据库连接池
        JdbcTemplate jdbcTemplate = new JdbcTemplate();
        jdbcTemplate.setDataSource(dataSource);
        return jdbcTemplate;
    }

    //创建事务管理器，注入数据库连接池
    @Bean//表示在IOC创建对象
    public DataSourceTransactionManager getDataSourceTransactionManager(DataSource dataSource){
        DataSourceTransactionManager transactionManager = new DataSourceTransactionManager();
        transactionManager.setDataSource(dataSource);
        return transactionManager;
    }
}
```

测试类：

```java
@Test
public void testMoney2(){
    //使用全注解方式
    ApplicationContext context = new AnnotationConfigApplicationContext(BeanConfig.class);
    UserService userService = context.getBean("userService", UserService.class);
    userService.transferAccount(1,2,100);
}
```

## 4.spring5新功能

### 4.1 自带通用的日志封装

支持使用log4j2

1.引入jar包：

![image-20220407194035435](./Spring5.assets\image-20220407194035435.png)

log4j换为2.15及以上版本

2.创建配置文件

模板复制的xml：

```xml
<?xml version="1.0" encoding="UTF-8"?>
<!--日志级别以及优先级排序: OFF > FATAL > ERROR > WARN > INFO > DEBUG > TRACE > ALL -->
<!--Configuration后面的status用于设置log4j2自身内部的信息输出，可以不设置，当设置成trace时，可以看到log4j2内部各种详细输出-->
<configuration status="INFO">
    <!--先定义所有的appender-->
    <appenders>
        <!--输出日志信息到控制台-->
        <console name="Console" target="SYSTEM_OUT">
            <!--控制日志输出的格式-->
            <PatternLayout pattern="%d{yyyy-MM-dd HH:mm:ss.SSS} [%t] %-5level %logger{36} - %msg%n"/>
        </console>
    </appenders>
    <!--然后定义logger，只有定义了logger并引入的appender，appender才会生效-->
    <!--root：用于指定项目的根日志，如果没有单独指定Logger，则会使用root作为默认的日志输出-->
    <loggers>
        <root level="info">
            <appender-ref ref="Console"/>
        </root>
    </loggers>
</configuration>
```

配置完成后运行主函数或者测试，控制台会出现日志记录

### 4.2 @Nullable

@Nullable 注解可以使用在方法上面，属性上面，参数上面，表示方法返回可以为空，属性值可以 为空，参数值可以为空

例如：

使用在方法上：

![image-20220407194445839](./Spring5.assets\image-20220407194445839.png)

使用在参数列表

![image-20220407194523326](./Spring5.assets\image-20220407194523326.png)

使用在属性值

![image-20220407194540848](./Spring5.assets\image-20220407194540848.png)

### 4.3  核心容器支持函数式风格 

我们正常创建一个对象不会交给spring管理，如果有这个需求，需要GenericApplicationContext：

```java
public class Generic {
    //new User();创建的对象不在IOC中
    //通过函数式编程创建对象，并交给spring的IOC管理
    @Test
    public void testGenericApplicationContext() {
        //首先需要创建一个GenericApplicationContext对象
        GenericApplicationContext context = new GenericApplicationContext();
        //把context内容清空，以便进行注册
        context.refresh();
        //向IOC注册bean，通过lambda表达式将函数传递
        context.registerBean("user111", User.class, () -> new User());
//        context.registerBean( User.class, () -> new User());
        //获取IOC中的对象，注意注册时如果没有写beanName参数，这里getBean要写全类路径
        User user = (User) context.getBean("user111");
//        User user = (User) context.getBean("com.almond.pojo.User");
        System.out.println(user);
    }
}
```

### 4.4 junit4和junit5整合

junit4：

```java
@RunWith(SpringJUnit4ClassRunner.class)
@ContextConfiguration("classpath:Bean1.xml")
public class AnnoTest {
    @Autowired
    private UserService userService;

    @Test
    public void testService(){
        userService.transferAccount(1,2,100);
    }
    @Test
    ....//我们不需要每个test都加载xml，从ApplicationContext取出待操作对象
}
```

junit5：

```java
@ExtendWith(SpringExtension.class)
@ContextConfiguration("classpath:Bean1.xml")
public class AnnoTestJunit5 {
    @Autowired
    private UserService userService;
    @Test
    public void test1(){
        userService.transferAccount(2,1,100);
    }
}
```

可以采用一个注解替换两个注解：

```java
@SpringJUnitConfig(locations = "classpath:bean1.xml")
public class AnnoTestJunit5 {
    @Autowired
    private UserService userService;
    @Test
    public void test1(){
        userService.transferAccount(2,1,100);
    }
}
```
