# javaweb

## 一.XML简介

## 1.概念

> 什么是XML文件？

定义：xml是可扩展的标记性语言

可扩展：标签可以自由指定

例如：

```xml
<bookstore>
<book category="CHILDREN">
  <title>Harry Potter</title> 
  <author>J K. Rowling</author> 
  <year>2005</year> 
  <price>29.99</price> 
</book>
<book category="WEB">
  <title>Learning XML</title> 
  <author>Erik T. Ray</author> 
  <year>2003</year> 
  <price>39.95</price> 
</book>
</bookstore> 
```

> xml的作用？

1.用于保存数据，而且这些数据具有自我描述性

2.可以作为项目或者模块的配置文件

3.作为网络传输数据的格式（现在大多用JSON传输）

## 2.语法

第一个xml文件：

```xml
<?xml version="1.0" encoding="utf-8" ?>
<!--xml的版本和xml文件本身使用的字符集-->
<books>
<!-- 很多图书信息   -->
    <book NO="1000000123">
    <!--   一本图书的信息，其中有编号NO     -->
        <name>时间简史</name>
        <author>霍金</author>
        <price>100</price>
    </book>

    <book NO="1000000125">
        <name>java</name>
        <author>tom</author>
        <price>125</price>
    </book>
</books>
```

标签名称不能以-xml开头，不能包含空格，对大小写敏感，

根元素时唯一的包含所有其他元素，

用&lt &gt表示小于号大于号，

```xml
<![CDATA[这里是纯文本不会被xml解析<xxx><xxx>]]>
```

## 3.解析

读取xml文件并传唤为我们需要的数据称为解析，使用w3c指定的dom技术进行解析



---



### 1.dom4j解析

dom4j读入xml文件

```java
 public void test1() throws Exception {
        //创建sax输入流
        SAXReader saxReader = new SAXReader();
        //从xml文件中读入数据
        Document document = saxReader.read("src/XML/books.xml");
        System.out.println(document);
        //org.dom4j.tree.DefaultDocument@3fb4f649 [Document: name src/XML/books.xml]
}
```

以上述第一个xml文件为例，读取创建对象Book

1.先创建Book对象，包括如下实例域，以及getset方法，构造器，tostring方法

```java
private String NO;
private String name;
private String author;
private BigDecimal price;
```

2.dom4j读入并生成对象：

```java
public void test2() throws Exception {
    SAXReader saxReader = new SAXReader();
    Document document = saxReader.read("src/XML/books.xml");
    //获取根元素
    Element rootElement = document.getRootElement();
    //elements方法:获取多个元素(book)，element方法当只有一个元素时可以使用
    List<Element> books = rootElement.elements("book");
    for (Element book : books){
        //一种拿到标签内信息的方法
        Element nameElement = book.element("name");
        String nameText = nameElement.getText();//nameText:时间简史 java
        //直接从book拿取信息
        String author = book.elementText("author");
        String price = book.elementText("price");
        String NO = book.attributeValue("NO");
        System.out.println(
            new Book(NO,nameText,author,new BigDecimal(price))
        );
        //Book{NO='1000000123', name='时间简史', author='霍金', price=100}
        //Book{NO='1000000125', name='java', author='tom', price=125}
    }
}
```

## 二.Tomcat

### 1.目录

> bin：存放Tomcat服务器的可执行文件
>
> conf：存放Tomcat服务器的配置文件
>
> lib：存放Tomcat服务器的jar包
>
> logs：存放Tomcat服务器运行时输出的日记信息
>
> temp：存放Tomcat运行时产生的临时数据
>
> webapps：存放部署的web工程
>
> work：是Tomcat工作时的目录，用来存放Tomcat运行时jsp翻译为servlet的源码，以及Session钝化的目录（钝化就是序列化，即把一个对象写入磁盘）

### 2.启动

> 方法1

打开bin目录的startup.bat，再访问http://localhost:8080/

> 方法2

cmd中：

```cmd
C:\Users\Alm0nd>cd D:\DevelopSoftware_root\java\apache-tomcat-8.0.42-windows-x64\apache-tomcat-8.0.42\bin
C:\Users\Alm0nd>d:
D:\DevelopSoftware_root\java\apache-tomcat-8.0.42-windows-x64\apache-tomcat-8.0.42\bin>catalina run
```

### 3.关闭

1.直接关闭startup.bat界面

2.在startup.bat界面ctrl+c

3.在Tomcat的bin目录中运行shutdown.bat

### 4.修改端口号

conf目录中找到server.xml，将其中Connector标签中的port属性改为新端口号

### 5.部署web工程到Tomcat中

方法1：

将项目文件拷贝至Tomcat目录的webapps下，webapps中每一个文件夹都是一个工程，这里放入book_static

![image-20220311095222776](.\imgs\image-20220311095222776.png)



> 启动tomcat， http://localhost:8080相当于webapps目录，在这里继续写要访问的界面
>
> http://localhost:8080/book_static/pages/user/regist.html
>
> 将URl中的localhost换成本机ip地址，其他人可以通过该url访问到该网站。

方法2：

在D:创建D:\bookstatic项目文件

找到Tomcat下的conf目录下的Catalina/localhost，新建一个book.xml文件，写入:

```
<Context  path="/book" docBase="D:\bookstatic" />
```

> Path：http://localhost:8080/book相当于映射到D:\bookstatic

**注意这个docBase文件路径不能有特殊字符，最好把工程放在目录简单的地方**

### 6.ROOT访问

Tomcat的webapps中默认有一些文件，其中包括了ROOT

当我们访问http://ip:port/ 没有指定工程名时，默认访问ROOT工程

当我们访问任何一个工程时（包括ROOT），如果url没有指定具体的页面，默认访问index文件。

### 7.整合Tomcat和idea

setting->bulid,execution,deployment->Application Servers，添加Tomcat文件目录即可。

> 创建一个包含Tomcat服务器的模块（module）
>
> new Module->Java Enterprise ->选择web application，选择application server为Tomcat服务器
>
> idea2021配置方法：https://blog.csdn.net/znufe_newer/article/details/113115002

### 8.目录结构

如图：

![image-20220311160912371](.\imgs\image-20220311160912371.png)

这里写的模块名也是web，不过下称web都是指选中的web文件夹，而不是模块名

src：存放java代码

web：存放html，css，js代码等，包括WEB-INF

WEB-INF：受服务器保护的目录，浏览器无法访问这个目录的内容

lib：存放第三方jar包，需要自己导包

打开WEB-INF，发现文件web.xml，该文件是整个动态web工程中的配置部署描述文件，可以在这里配置web工程的组件。例如：

Servlet程序，Filter过滤器，Listener监听器，Session超时等等。

![image-20220311161354554](.\imgs\image-20220311161354554.png)

### 9.导入jar包

先将jar包复制到lib中

![image-20220311163648061](.\imgs\image-20220311163648061.png)

打开project structure

![image-20220311163711587](.\imgs\image-20220311163711587.png)

libaraies-> + -> java

![image-20220311163913761](.\imgs\image-20220311163913761.png)

选择lib，以及要导入的模块就好了，完成后可以修改name

![image-20220311164012367](.\imgs\image-20220311164012367.png)

### 10.修改tomcat

![image-20220311164202108](.\imgs\image-20220311164202108.png)

#### 1）建议修改tomcat名（Name）为当前模块名

![image-20220311164233879](.\imgs\image-20220311164233879.png)

 

---



#### 2）可以修改默认访问地址

![image-20220311164545400](.\imgs\image-20220311164545400.png)



---



#### 3）建议修改工程路径,与模块名保持一致

找到Application context修改为/模块名

![image-20220311165301967](.\imgs\image-20220311165301967.png)

修改后发现url改变：

![image-20220311165348654](.\imgs\image-20220311165348654.png)

#### 4）修改时部署（热部署）

做修改后服务器会自动重新部署：

![image-20220311165913444](.\imgs\image-20220311165913444.png)

## 三.Servlet

### 1.概述

> Servlet是javaEE规范之一，规范就是接口
>
> Javaweb三大组件之一，三大组件是Servlet程序，Filter过滤器，Listener监听器
>
> 是运行在服务器上的一个java程序，负责接收客户端穿来的请求，并响应数据给客户端

### 2.第一个Servlet程序

#### 1）从tomcat中导入Servlet包：

参考https://www.cnblogs.com/fengnan/p/9311943.html

![image-20220311171843365](.\imgs\image-20220311171843365.png)

打开library，加入servlet即可

![image-20220311171902874](.\imgs\image-20220311171902874.png)

#### 2）新建一个类实现Servlet接口

```java
public class FirstServlet implements Servlet {

    @Override
    public void init(ServletConfig servletConfig) throws ServletException {

    }

    @Override
    public ServletConfig getServletConfig() {
        return null;
    }
	
    //service方法是每次servlet被访问时自动调用的方法,用于获取请求并响应请求
    @Override
    public void service(ServletRequest servletRequest, ServletResponse servletResponse) throws ServletException, IOException {
        System.out.println("servlet被访问了");
    }

    @Override
    public String getServletInfo() {
        return null;
    }

    @Override
    public void destroy() {

    }
}
```

#### 3）配置web.xml

在webapp标签中配置：

```xml
<servlet>
    <!--    分别配置为类名和包名    -->
    <servlet-name>FirstServlet</servlet-name>
    <servlet-class>com.almond.demo.FirstServlet</servlet-class>
</servlet>

<!--  配置servlet对应哪个url地址  -->
<servlet-mapping>
    <!--    要配置的servlet的名字，就是上面的FirstServlet    -->
    <servlet-name>FirstServlet</servlet-name>
    <!--    配置当访问哪个url时，调用该servlet?    -->
    <!--    /hello紧跟在默认url后，即http://IP:port/工程路径/hello    -->
    <url-pattern>/hello</url-pattern>
</servlet-mapping>
```

启动tomcat服务器，输入url：

![image-20220311173331376](.\imgs\image-20220311173331376.png)

显示：

![image-20220311173321691](.\imgs\image-20220311173321691.png)

访问的原理：

![image-20220311180328071](.\imgs\image-20220311180328071.png)

### 3.Servlet的生命周期

> 1.执行Servlet构造器方法
>
> 2.执行init初始化
>
> ​	**1和2是在第一次访问的时候创建servlet程序时调用**
>
> 3.执行service方法，每次访问时都会调用
>
> 4.destroy方法：在web工程停止时调用

访问三次hello页面：

![image-20220311200734568](.\imgs\image-20220311200734568.png)

关闭服务器后：

![image-20220311200654055](.\imgs\image-20220311200654055.png)



### 4.获取请求方式

有两种请求get和post，我们需要分辨两者，并对其进行不同的响应

现在有getPostTest.html文件：

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
    <form action="http://localhost:8080/web/hello" method="post">
        <input type="submit">
    </form>
</body>
</html>
```

我们需要对http://localhost:8080/web/hello对应的servlet类的service方法进行修改：

```java
@Override
public void service(ServletRequest servletRequest, ServletResponse servletResponse) throws ServletException, IOException {
    //HttpServletRequest是ServletRequest的子类，它拥有getMethod()方法，可以查看请求方式
    HttpServletRequest httpServletRequest = (HttpServletRequest) servletRequest;
    //我们拿到method是字符串POST或者GET
    String method = httpServletRequest.getMethod();
    if("POST".equals(method)){
        System.out.println("post请求");
    }else if("GET".equals(method)){
        System.out.println("get请求");
    }
}
```

### 5.继承HttpServlet类实现请求响应

实际开发中使用的方法是继承HttpServlet类实现请求响应

编写一个类继承HttpServlet

```java
public class SecondServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        System.out.println("get");
        super.doGet(req, resp);
    }

    @Override
    protected void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        System.out.println("post");
        super.doPost(req, resp);
    }
}
```

配置web.xml：

```xml
<servlet>
    <servlet-name>SecondServlet</servlet-name>
    <servlet-class>com.almond.demo.SecondServlet</servlet-class>
</servlet>

<servlet-mapping>
    <servlet-name>SecondServlet</servlet-name>
    <url-pattern>/hello2</url-pattern>
</servlet-mapping>
```

编写html：

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
    <form action="http://localhost:8080/web/hello2" method="POST">
        <input type="submit">
    </form>
</body>
</html>
```

在http://localhost:8080/web/getPostTest.html提交后：

![image-20220311201958134](.\imgs\image-20220311201958134.png)

### 6.idea自动生成Servlet

1.创建类

![image-20220311202254094](.\imgs\image-20220311202254094.png)

2.设置

![image-20220311202330815](.\imgs\image-20220311202330815.png)

name：对应配置xml文件中的<servlet-name>标签

Class：类名

**取消勾选注解功能**

3.配置xml映射

```xml
<servlet-mapping>
    <servlet-name>ThirdServlet</servlet-name>
    <url-pattern>/hello3</url-pattern>
</servlet-mapping>
```

### 7.Servlet类的继承体系

![image-20220311204318724](.\imgs\image-20220311204318724.png)

### 8.ServletConfig

在xml中配置初始参数

```xml
<servlet>
    <!--    分别配置为类名和包名    -->
    <servlet-name>FirstServlet</servlet-name>
    <servlet-class>com.almond.demo.FirstServlet</servlet-class>
    <init-param>
        <param-name>name1</param-name>
        <param-value>1</param-value>
    </init-param>
    <init-param>
        <param-name>name2</param-name>
        <param-value>2</param-value>
    </init-param>
</servlet>
```

在FirstServlet中修改init方法

```java
@Override
public void init(ServletConfig servletConfig) throws ServletException {
    System.out.println("init被调用");
    //获取xml中的name，也就是类名
    System.out.println(servletConfig.getServletName());
    //获取xml中的初始化参数，这个需要自己在xml中编写
    System.out.println(servletConfig.getInitParameter("name1"));
    System.out.println(servletConfig.getInitParameter("name2"));
    //获取ServletContext
    System.out.println(servletConfig.getServletContext());
}
```

结果为：

![image-20220311205517027](.\imgs\image-20220311205517027.png)

注意：

每个Servlet对象都有自己的ServletCofig。

注意在重写init(ServletConfig config)方法时，调用supr.init(config)，因为父类中做了对config的保存操作，子类方法如果没做这一操作又没有调用父类的方法，使用时可能获取不到config而报空指针异常。

### 9.ServletContext类

#### 1）概述

> 1.ServletContext是一个接口，表示Servlet上下文对象
>
> 2.一个web工程，只有一个ServletContext对象实例
>
> 3.ServletContext对象是一个域对象
>
> 什么是域对象？
>
> 可以像Map一样存取数据的对象，叫域对象。
>
> 域指的是存取数据的操作范围：整个web工程

域对象的方法：setAttribute()	getAttribute()	removeAttribute()

#### 2）四个作用

> 获取工程的xml文件中的上下文参数 context-param
>
> 获取当前工作路径
>
> 获取工程部署在服务器硬盘的绝对路径
>
> 像Map一样存取数据



##### （1）对前三个作用的测试

1.配置xml中的context-param，在web-app标签中写入：

```xml
<!--  整个工程都可以访问contex-param  -->
<context-param>
    <param-name>username</param-name>
    <param-value>almond</param-value>
</context-param>

<context-param>
    <param-name>password</param-name>
    <param-value>994156</param-value>
</context-param>
```

2.创建对应的对象以及配置xml：（xml略）

```java
public class ContextServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        //需要拿到config后获取context
        ServletContext context = getServletConfig().getServletContext();
        //获取参数
        System.out.println(context.getInitParameter("username"));
        System.out.println(context.getInitParameter("password"));
        //获取当前工程路径
        System.out.println(context.getContextPath());
        //获取工程的绝对路径
        // "/"指的是 "http://ip:port/工程名/" 末尾的"/"
        System.out.println(context.getRealPath("/"));
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

    }
}
```

3.结果：

![image-20220311214149340](.\imgs\image-20220311214149340.png)



---



访问绝对路径，发现就是把web文件夹换了个名字，其余内容完全一致

![image-20220311214908085](.\imgs\image-20220311214908085.png)

原工程的路径 D:\DevelopCode_root\Java\XML\web\web

真实路径 D:\DevelopCode_root\Java\XML\web\target\web-1.0-SNAPSHOT\

可见idea生成了一个对原工程的复制，而且这个复制随着我们对原工程的修改而修改。



##### （2）对存取数据的测试

创建两个类：

Context负责写和读：

```java
public class Context1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        ServletContext context = getServletContext();
        System.out.println("context1在创建前访问属性值"+context.getAttribute("user"));
        context.setAttribute("user","xxxxx");
        System.out.println("context1在创建后访问属性值"+context.getAttribute("user"));
        System.out.println("context1中的context地址是"+context);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

    }
}
```

Context2负责读：

```java
public class Context2 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {
        ServletContext context = getServletContext();
        System.out.println("context2访问属性值"+context.getAttribute("user"));
        System.out.println("context2中context的地址是"+context);
    }

    @Override
    protected void doPost(HttpServletRequest request, HttpServletResponse response) throws ServletException, IOException {

    }
}
```

为两个类配置映射，**注意映射中<url-pattern>/context1</url-pattern>一定要加/**

启动服务器按顺序访问context1，context2

![image-20220311221826347](.\imgs\image-20220311221826347.png)

![image-20220311221852576](.\imgs\image-20220311221852576.png)

可见访问的是同一个context对象，这个对象设置的属性值在未被创建前为空。当我们此时重启服务器，直接访问context2

![image-20220311222052488](.\imgs\image-20220311222052488.png)

说明这个设置的属性值不是写在硬盘里的，类似内存，断电丢失

###  10.HTTP协议

![image-20220312102054615](.\imgs\image-20220312102054615.png)

响应状态码：

200 请求成功

302 请求重定向

404 服务器收到请求，但客户端请求的数据不存在（请求地址错误）

500 服务器已经收到请求，但是服务器内部错误（通常是服务器的代码错误）

### 11.HttpServletRequest类

当有请求进入服务器时，服务器将请求过来的HTTP协议信息解析封装到Request对象中，然后传递到service方法（doGet,doPost方法）中给我们使用。我们通过HttpServletRequest对象，获取到所有请求的信息。

####  1.常用api

```java
public class HttpTest extends HttpServlet {
@Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
    System.out.println("url: "+req.getRequestURL());
    System.out.println("uri: "+req.getRequestURI());
    System.out.println("client ip:"+req.getRemoteHost());
    //使用localhost或者127.0.0.1访问ip都是127.0.0.1
    //使用ip访问才能得到真实的ip地址
    System.out.println("请求头User-Agent:"+req.getHeader("User-Agent"));

    System.out.println("请求方式get?post:"+req.getMethod());

    }
}
```

使用真实ip访问，结果：

```
url: http://192.168.0.106:8080/web2/httptest
uri: /web2/httptest
client ip:192.168.0.106
请求头User-Agent:Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:98.0) Gecko/20100101 Firefox/98.0
请求方式get?post:GET
```

请求头验证：

![image-20220314172609093](.\imgs\image-20220314172609093.png)

#### 2.获取请求参数

html：

```html
<body>
    <form action="http://localhost:8080/web2/parameterServlet" method="get">
        用户名<input type="text" name="username"><br/>
        密码<input type="password" name="password"><br/>
        <input type="checkbox" name="hobby" value="c++">C++
        <input type="checkbox" name="hobby" value="java">java
        <input type="checkbox" name="hobby" value="js">js<br/>
        <input type="submit">
    </form>
</body>
```

xml：

```xml
<servlet>
    <servlet-name>ParameterServlet</servlet-name>
    <servlet-class>com.almond.demo01.ParameterServlet</servlet-class>
</servlet>

<servlet-mapping>
    <servlet-name>ParameterServlet</servlet-name>
    <url-pattern>/parameterServlet</url-pattern>
</servlet-mapping>
```

java：

```java
public class ParameterServlet extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        String username = req.getParameter("username");
        String password = req.getParameter("password");
        String[] hobby = req.getParameterValues("hobby");
        System.out.println("用户名："+username);
        System.out.println("密码："+password);
        System.out.println("爱好："+ Arrays.asList(hobby));
    }
}
```

结果：

```
用户名：almond
密码：xxzxz
爱好：[c++, java, js]
```

> 注意使用post请求会出现中文乱码问题
>
> 使用方法req.setCharaterEncoding("UTF-8")解决，注意该方法要写在**所有**获取请求参数的方法之前，否则无效

#### 3.请求的转发

服务器收到请求之后，从一个资源跳转至另一个资源叫请求的转发。

![image-20220315120354660](.\imgs\image-20220315120354660.png)

例如：

Servlet1:

```java
public class Servlet1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //获取参数，处理servlet1的业务
        System.out.println("servlet1获取参数："+req.getParameter("username"));
        //盖章表示完成了servlet1的业务
        req.setAttribute("servlet1Key","servlet1的盖章");
        //问路，写入servlet2的地址
        RequestDispatcher requestDispatcher = req.getRequestDispatcher("/servlet2");
        //跳转到servlet2
        requestDispatcher.forward(req,resp);
    }
}
```

Servlet2:

```java
public class Servlet2 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //获取参数
        System.out.println("servlet2获取参数"+req.getParameter("username"));
        //检查是否经过servlet1的处理
        Object servletKey = req.getAttribute("servletKey");
        System.out.println("判断是否经由servlet1处理");
        //处理自己的业务
        System.out.println("servlet2处理自己的业务");
    }
}
```

web.xml:

```xml
<servlet>
    <servlet-name>Servlet1</servlet-name>
    <servlet-class>com.almond.demo01.Servlet1</servlet-class>
</servlet>
<servlet-mapping>
    <servlet-name>Servlet1</servlet-name>
    <url-pattern>/servlet1</url-pattern>
</servlet-mapping>

<servlet>
    <servlet-name>Servlet2</servlet-name>
    <servlet-class>com.almond.demo01.Servlet2</servlet-class>
</servlet>
<servlet-mapping>
    <servlet-name>Servlet2</servlet-name>
    <url-pattern>/servlet2</url-pattern>
</servlet-mapping>
```

在浏览器地址栏，参数列表用?分隔开

![image-20220315122537755](.\imgs\image-20220315122537755.png)

res:

```
servlet1获取参数："almond"
servlet2获取参数"almond"
判断是否经由servlet1处理
servlet2处理自己的业务
```

> 请求转发的特点
>
> 1.当发生请求转发时，浏览器地址栏没有发生变换
>
> 2.请求转发是一次请求
>
> 3.共享Request域中的数据
>
> 4.可以转发到WEB-INF目录下的界面(正常地填写url是无法访问该文件中的html的)
>
> 5.不可以跳转到web工程以外的资源，因为默认url是当前工程目录，如果输入一个外界url，会自动拼接到默认路径

#### 4.base标签

##### （1）题外话，路径的具体写法

项目的目录：

![image-20220315153444611](.\imgs\image-20220315153444611.png)

以index.html为例：

```html
<a href="http://localhost:8080/web2/a/b/c/base.html">跳转至base页面</a><br/>
<a href="a/b/c/base.html">相对路径，跳转至base界面</a><br/>
```

**可以发现，绝对路径是不写web文件夹的，而在浏览器url栏中显示的也是：**

```
http://localhost:8080/web2/index.html
```

可以理解为web2文件夹就代替了web文件夹



----



要访问一个servlet界面：

web.xml文件中的映射为

```xml
<servlet>
    <servlet-name>BaseTest</servlet-name>
    <servlet-class>com.almond.demo01.BaseTest</servlet-class>
</servlet>
<servlet-mapping>
    <servlet-name>BaseTest</servlet-name>
    <url-pattern>/base</url-pattern>
</servlet-mapping>
```

在xml中的/base是指

```
http://localhost:8080/web2/base
```

在index.html中想要访问这个文件不可以写成这样：

```html
<a href="/base">相对路径，请求转发到base页面(错误的)</a><br/>
含义是
http://localhost:8080/base
```

正确的写法:

```html
<a href="/web2/base">请求转发到base页面(正确的相对路径)</a>
```

总结一下：

![image-20220315160555660](.\imgs\image-20220315160555660.png)



##### （2）base标签的作用

index.html中

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>主页</title>
</head>
<body>
    这里是主页
    <a href="http://localhost:8080/web2/a/b/c/base.html">跳转至base页面</a><br/>
    <a href="a/b/c/base.html">相对路径，跳转至base界面</a><br/>

    <a href="http://localhost:8080/web2/base">绝对路径，请求转发到base页面（能跳转但回不来）</a> <br/>
    <a href="/web2/base">相对路径，请求转发到base页面（能跳转但回不来）</a>

</body>
</html>
```

web.xml:

```xml
<servlet>
    <servlet-name>BaseTest</servlet-name>
    <servlet-class>com.almond.demo01.BaseTest</servlet-class>
</servlet>
<servlet-mapping>
    <servlet-name>BaseTest</servlet-name>
    <url-pattern>/base</url-pattern>
</servlet-mapping>
```

BaseTest:

```java
public class BaseTest extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        System.out.println("经过请求转发");
        req.getRequestDispatcher("/a/b/c/base.html").forward(req,resp);
    }
}
```

base.html:

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
    这里是a/b/c/base页面
    <a href="../../../index.html">返回主页面</a>
</body>
</html>
```

问题在于：我们可以从正常跳转界面返回index.html，但是请求转发不能，分组转发返回的url为：

```
http://localhost:8080/index.html

http://localhost:8080/web2/index.html//正确的
```

原因在于相对路径的工作方式是在当前浏览器的url中进行操作,请求转发的url是

```
http://localhost:8080/web2/base

具体步骤如下：

../ 一次：
http://localhost:8080/web2

../ 两次：
http://localhost:8080

../ 三次：到头了，总不能把ip和端口号都干掉吧
http://localhost:8080

加上index.html:
http://localhost:8080/index.html
```



解决方法：html文件中加base标签

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
    <base href="http://localhost:8080/web2/a/b/c/">//在head中加入base标签
    //注意路径写到文件夹即可
</head>
<body>
    这里是a/b/c/base页面
    <a href="../../../index.html">返回主页面</a>
</body>
</html>
```

### 12.HttpServletResponse类

#### 1.字节流和字符流

getOutputStream() 多用于下载

getWriter() 多用于向客户端回传字符串

两个流只能使用一个，不能同时使用



```java
public class Response extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        PrintWriter writer = resp.getWriter();
        writer.write("response响应");
    }
}
```

访问结果：

![image-20220315161513034](.\imgs\image-20220315161513034.png)

可见中文会乱码，解决方法：

```java
public class Response extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //设置响应的字符集
        //resp.setCharacterEncoding("UTF-8");
        //仅仅设置响应头还不够，客户端的浏览器字符集可能不是UTF-8
        //我们需要通过响应头的参数来提示客户端的浏览器使用UTF-8字符集
        resp.setContentType("text/html;charset=utf8");
		//setContentType方法要在获取流对象前写好，而且自动将服务器，客户端，响应的字符集全部修改
        
        PrintWriter writer = resp.getWriter();
        writer.write("响应的文字");
    }
}
```

#### 2.请求重定向

![image-20220315165204125](.\imgs\image-20220315165204125.png)



第一种方法的代码：（不建议用）

```java
public class Response1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //设置响应状态码302，表示已经搬迁，需要重定向
        resp.setStatus(302);
        resp.setHeader("location","http://localhost:8080/web2/response2");
    }
}
```

---

```java
public class Response2 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        PrintWriter writer = resp.getWriter();
        writer.write("response2");
    }
}
```

> 特点：
>
> 1.浏览器地址栏发生变化
>
> 2.两次请求
>
> 3.不共享request域中的数据（1界面中req.setAttribute(),2界面无法得到）
>
> 4.不能重定向到WEB-INF，因为重定向仍然是客户端向服务器发请求
>
> 5.可以访问工程外的资源



第二种重定向的方法：

```java
public class Response1 extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //设置响应状态码302，表示已经搬迁，需要重定向
        //resp.setStatus(302);
		//resp.setHeader("location","http://localhost:8080/web2/response2");
        
        resp.sendRedirect("http://localhost:8080/web2/response2");
    }
}
```

## 四.项目第二阶段

### 1.javaEE的三层架构

![image-20220315171941805](.\imgs\image-20220315171941805.png)

项目中的包名：

![image-20220315172645196](.\imgs\image-20220315172645196.png)

### 2.导入jar包

https://www.bilibili.com/video/BV1Y7411K7zz?p=165

### 3.新版idea创建web模块

https://blog.csdn.net/llplllll/article/details/116903198

## 五.JSP

jsp是为了解决Servlet回传HTML界面的问题（需要一行一行写前端代码，代码量大，成本高不易维护）

jsp本质是一个Servlet程序，继承了HttpServlet类，具体实现也是一行一行写前端代码输出流，回传到客户端。

### 1.page指令

如下代码第一行就是page指令：

```jsp
<%@ page contentType="text/html;charset=UTF-8" language="java" %>
<html>
<head>
    <title>Title</title>
</head>
<body>

</body>
</html>
```



---



设置界面出错自动跳转：

```jsp
<%@ page contentType="text/html;charset=UTF-8" language="java"
    errorPage="/error.jsp"
%>
```

当前界面出错则跳转至 http://ip:port/工程名/error.jsp



导包：

```jsp
<%@ page import="java.util.Arrays" %>
```



创建异常对象：

```jsp
<%@ page contentType="text/html;charset=UTF-8" language="java"
    isErrorPage="true"
%>
```



### 2.脚本

声明脚本：（基本不用）

在其中声明的内容是在jsp文件中的类中

```jsp
<%!
   写java代码 
%>
```

可以声明类的属性，静态代码块，类的方法，内部类



表达式脚本：

```jsp
<%=表达式%>
```

可以在jsp页面上输出数据。

注意：

表达式中的内容被写入到输出流中 :out.print(表达式)，所以不要以分号结束

表达式被写入到_jspService方法中，该方法中的参数可以调用，比如表达式写成request.getParameter("xxx")



代码脚本：

```
<%
	java语句
%>
```

经常用于写java语句，比如：

```
<%
	int i = 1;
	if(i==1) System.Out.println(i) //输出到控制台
%>
```



被写入到_jspService方法中，该方法中的参数可以调用

可以多个块拼接成一个java语句

```
<%
	int i = 1;
	if(i==1) {System.Out.println(i) //输出到控制台
%>
<%
	} 
%>
```

### 3.注释

html注释会被当做输出内容传递到界面上

java注释//和/* */都会被翻译到jsp对应的java文件中

```jsp
<%-- jsp注释 --%>
```

jsp注释可以注释掉上述的其他两种注释

### 4.九大对象

request	请求对象

response	响应对象

pageContext	jsp上下文对象

session	会话对象

application	ServletContext对象

config	ServletConfig对象

out	jsp输出流对象

page	指向当前jsp界面的对象

exception	异常对象，在page中设置isErrorPage=true

### 5.四大对象

四个域对象分别是： 

pageContext (PageContextImpl 类) 当前 jsp 页面范围内有效，换界面就没有了



request (HttpServletRequest 类)、 一次请求内有效，请求转发算是一次请求

 

session (HttpSession 类)、 一个会话范围内有效（打开浏览器访问服务器，直到关闭浏览器） 



application (ServletContext 类) 整个 web 工程范围内都有效（只要 web 工程不停止，数据都在）



四个域对象使用顺序：pageContext request session application 

尽量早释放域对象

### 6.输出

有两种输出方法，out和response，两者拥有属于自己的缓冲区，当jsp界面加载完成，会调用out.flush方法将out缓冲输入到response缓冲中。

```
<%
    out.write("out的输出1<br/>");
    out.flush();//将write缓存中的内容输入到response缓存中
    response.getWriter().write("response输出1<br/>");
    response.getWriter().write("response输出2<br/>");
    out.write("out的输出2<br/>");
%>

out的输出1
response输出1
response输出2
out的输出2
```

jsp源码中使用out进行输出，为了不打乱程序输出顺序，我们也使用out输出

out.write()	只能输出字符串，会强制将数据转换为char放入数组输出

out.print()	输出任何数据，因为会将数据转换为字符串调用write

使用out.print()方法输出数据

### 7.常用标签

#### （1）静态包含(常用，因为动态的活被抢了)

在主界面写入

```
主界面的内容1<br>
<%@ include file="/test/include.jsp" %><br>
主界面的内容2
    
主界面的内容1
main包含的子页面
主界面的内容2 
```

在主界面对应位置就会出现子界面的内容，原理就是把子界面的输出代码拷贝到静态包含的主界面的对应的位置

#### （2）动态包含

主界面：

```jsp
主界面的内容1<br>
<jsp:include page="/test/include.jsp">
    <jsp:param name="username" value="almond"></jsp:param>
    <jsp:param name="password" value="123456"></jsp:param>
</jsp:include>
<br>主界面的内容2
```

子界面：

```jsp
动态包含的子界面
<%=request.getParameter("username")%><br>
<%=request.getParameter("password")%>
```

输出结果：

```
主界面的内容1
动态包含的子界面 almond
123456
主界面的内容2 
```

动态包含会创建新的java文件，然后在jsp主界面的java文件中调用新文件，将原java的对象传输给新java，两者使用的是一个对象的缓冲区，保证了内容输出到界面的顺序。

动态界面可以传参数

####  （3）请求转发

```jsp
<jsp:forward page=""></jsp:forward>
```

### 8.Listener监听器

1、Listener 监听器它是 JavaWeb 的三大组件之一。JavaWeb 的三大组件分别是：Servlet 程序、Filter 过滤器、Listener 监听器。

 2、Listener 它是 JavaEE 的规范，就是接口

 3、监听器的作用是，监听某种事物的变化。然后通过回调函数，反馈给客户（程序）去做一些相应的处理。



#### ServletContextListener

 使用方法：

创建java类实现接口：

```java
public class ContextListenerTest implements ServletContextListener {
    @Override
    public void contextInitialized(ServletContextEvent sce) {
        System.out.println("初始化监听器");
    }

    @Override
    public void contextDestroyed(ServletContextEvent sce) {
        System.out.println("销毁监听器");
    }
}
```

web.xml:

```xml
<listener>
    <listener-class>com.almond.jsp.ContextListenerTest</listener-class>
</listener>
```

配置完成。



启动服务器，Output中：

```
...
22-Mar-2022 15:57:03.938 信息 [RMI TCP Connection(3)-127.0.0.1] org.apache.jasper.servlet.TldScanner.scanJars At least one JAR was scanned for TLDs yet contained no TLDs. Enable debug logging for this logger for a complete list of JARs that were scanned but no TLDs were found in them. Skipping unneeded JARs during scanning can improve startup time and JSP compilation time.
初始化监听器
[2022-03-22 03:57:03,979] Artifact jsp:war exploded: Artifact is deployed successfully
....
```

关闭服务器，Output中：

```
...
22-Mar-2022 15:58:29.715 信息 [main] org.apache.catalina.core.StandardService.stopInternal Stopping service Catalina
销毁监听器
22-Mar-2022 15:58:29.733 信息 [main] org.apache.coyote.AbstractProtocol.stop Stopping ProtocolHandler ["http-apr-8080"]
...
```

### 9.EL表达式

EL 表达式的全称是：Expression Language。是表达式语言。

EL 表达式的什么作用：EL 表达式主要是代替 jsp 页面中的表达式脚本在 jsp 页面中进行数据的输出。 因为 EL 表达式在输出数据的时候，比 jsp 的表达式脚本要简洁

#### （1）EL表达式使用方法

```
<body>
<%
request.setAttribute("key","值");
%>
表达式脚本输出 key 的值是：
<%=request.getAttribute("key1")==null?"":request.getAttribute("key1")%><br/>
EL 表达式输出 key 的值是：${key1}

</body>
```

EL 表达式的格式是：${表达式} 

EL 表达式在输出 null 值的时候，输出的是空串。jsp 表达式脚本输出 null 值的时候，输出的是 null 字符串。

#### （2）EL搜索域的顺序

EL 表达式主要是在 jsp 页面中输出数据，主要是输出域对象中的数据。



当四个域中都有相同的 key 的数据的时候，EL 表达式会按照四个域的从小到大的顺序去进行搜索，找到就输出。

优先级 request>session>application>pageContext

#### （3）EL表达式对对象的输出

提供了Person类，EL输出是针对get方法的，一个没有get方法的私有对象将无法输出，其次关注一下数组，ArrayList，Map的输出格式

```jsp
<body>
<%
    Person person = new Person();
    person.setName("国哥好帅！");
    person.setPhones(new String[]{"18610541354", "18688886666", "18699998888"});
    List<String> cities = new ArrayList<String>();
    cities.add("北京");
    cities.add("上海");
    cities.add("深圳");
    person.setCities(cities);
    Map<String, Object> map = new HashMap<>();
    map.put("key1", "value1");
    map.put("key2", "value2");
    map.put("key3", "value3");
    person.setMap(map);
    pageContext.setAttribute("p", person);
%>
输出 Person：${ p }<br/>
输出 Person 的 name 属性：${p.name} <br>
输出 Person 的 pnones 数组属性值：${p.phones[2]} <br>
输出 Person 的 cities 集合中的元素值：${p.cities} <br>
输出 Person 的 List 集合中个别元素值：${p.cities[2]} <br>
输出 Person 的 Map 集合: ${p.map} <br>
输出 Person 的 Map 集合中某个 key 的值: ${p.map.key3} <br>
输出 Person 的 age 属性：${p.age} <br>
</body>
```

#### （4）运算

##### 1.基本运算

基本运算格式：${运算表达式}  例如：${ 5 == 5 }

跟java没什么差别。



##### 2.empty运算

empty 运算可以判断一个数据是否为空，如果为空，则输出 true,不为空输出 false。 

以下几种情况为空：

1、值为 null 值的时候，为空 

2、值为空串的时候，为空 

3、值是 Object 类型数组，长度为零的时候

4、list 集合，元素个数为零 

5、map集合，元素个数为零

```jsp
<body>
<%
    // 1、值为 null 值的时候，为空
    request.setAttribute("emptyNull", null);
// 2、值为空串的时候，为空
    request.setAttribute("emptyStr", "");
// 3、值是 Object 类型数组，长度为零的时候
    request.setAttribute("emptyArr", new Object[]{});
// 4、list 集合，元素个数为零
    List<String> list = new ArrayList<>();
// list.add("abc");
    request.setAttribute("emptyList", list);
// 5、map 集合，元素个数为零
    Map<String, Object> map = new HashMap<String, Object>();
// map.put("key1", "value1");
    request.setAttribute("emptyMap", map);
%>
${ empty emptyNull } <br/>
${ empty emptyStr } <br/>
${ empty emptyArr } <br/>
${ empty emptyList } <br/>
${ empty emptyMap } <br/>
</body>
```

##### 3.三元表达式

```jsp
${ 12 != 12 ? "鉴定为真":"鉴定为假" 
```

##### 4.点运算和中括号

.点运算，可以输出 Bean 对象中某个属性的值。 []中括号运算，可以输出有序集合中某个元素的值并且[]中括号运算，还可以输出 map 集合

```jsp
<body>
<%
    Map<String, Object> map = new HashMap<String, Object>();
    map.put("a.a.a", "aaaValue");
    map.put("b+b+b", "bbbValue");
    map.put("c-c-c", "cccValue");
    request.setAttribute("map", map);
%>
${ map['a.a.a'] } <br>
${ map["b+b+b"] } <br>
${ map['c-c-c'] } <br>
</body>
```

####  （5）EL中的11个对象

EL 个达式中 11 个隐含对象，是 EL 表达式中自己定义的，可以直接使用。

pageContext：PageContextImpl类型 它可以获取 jsp 中的九大内置对象 



pageScope Map 	它可以获取 pageContext 域中的数据 

requestScope Map 	它可以获取 Request 域中的数据 

sessionScope Map 	它可以获取 Session 域中的数据 

applicationScope Map 	它可以获取 ServletContext 域中的数据

 

param Map 	它可以获取请求参数的值 

paramValues Map 它也可以获取请求参数的值，获取多个值的时候使用。



header Map 它可以获取请求头的信息 

headerValues Map 它可以获取请求头的信息，它可以获取多个值的情况 



cookie Map 它可以获取当前请求的 Cookie 信息 

initParam Map 它可以获取在 web.xml 中配置的上下文参数

##### 1.四个特定域

```jsp
<body>
<%
    pageContext.setAttribute("key1", "pageContext1");
    pageContext.setAttribute("key2", "pageContext2");
    request.setAttribute("key2", "request");
    session.setAttribute("key2", "session");
    application.setAttribute("key2", "application");
%>
${ applicationScope.key2 }
</body>
```

##### 2.pageContext

```jsp
<body>
1.协议： ${ pageContext.request.scheme }<br>
2.服务器 ip：${ pageContext.request.serverName }<br>
3.服务器端口：${ pageContext.request.serverPort }<br>
4.获取工程路径：${ pageContext.request.contextPath }<br>
5.获取请求方法：${ pageContext.request.method }<br>
6.获取客户端 ip 地址：${ pageContext.request.remoteHost }<br>
7.获取会话的 id 编号：${ pageContext.session.id }<br>
</body>
```

##### 3.param

```jsp
<body>
<%--param=?username=%22tom%22&username=%22jack%22--%>
    ${param.username}<br>
    ${paramValues.username}<br>
<%--   "tom" --%>
<%--[Ljava.lang.String;@2f2f489--%>
    ${paramValues.username[0]}<br>
    ${paramValues.username[1]}<br>
<%--    "tom"--%>
<%--    "jack"--%>
</body>
```

##### 4.header

获取http协议的请求头，注意User-Agent有特殊字符，需要中括号加引号处理

```jsp
${header['User-Agent']}
${header.Connection}

${headerValues['User-Agent'][0]}
```

##### 5.cookie

```jsp
    ${cookie} <br>
<%--{Idea-834a1989=javax.servlet.http.Cookie@6453ba67, --%>
<%--Idea-834a15c8=javax.servlet.http.Cookie@5b57aaac, --%>
<%--JSESSIONID=javax.servlet.http.Cookie@5fdae35e}--%>

<%--根据属性自动调用cookie对应的get方法--%>
${cookie.JSESSIONID.name} <br>
${cookie.JSESSIONID.value}
<%--JSESSIONID--%>
<%--4812C11BE86192E309C626BB93E58E13--%>
```

##### 6.initParam

web.xml中：

```jsp
<context-param>
    <param-name>url</param-name>
    <param-value>jdbc:mysql:///test</param-value>
</context-param>
```

在jsp中

```jsp
${initParam}
<%--{url=jdbc:mysql:///test}--%>
```

## 六.jstl

```jsp
<%--scopp指定范围，page,request,session,application val为键 value为值    --%>
<c:set scope="request" var="abc" value="123"/>
${requestScope.abc}

123
```

## 七.文件上传下载

### 1.文件上传

1.一个form标签method=post

2.form标签的encType属性值为multipart/form-data值

3.在form标签中使用input type=file添加上传的文件

4.编写服务器代码接收，处理上传的数据



导入包commons-fileupload commons-io

在java文件中：

```java
@Override
protected void doPost(HttpServletRequest req, HttpServletResponse resp) {
    //创建fileItem的工厂类
    FileItemFactory fileItemFactory=new DiskFileItemFactory();
    //创建解析数据的工具类
    ServletFileUpload servletFileUpload = new ServletFileUpload(fileItemFactory);
    try {
        //传入req,解析数据
        List<FileItem> list = servletFileUpload.parseRequest(req);
        for (FileItem fileItem : list){
            //判断是否是普通的表单项
            if(fileItem.isFormField()){
                System.out.println("表单的属性值"+fileItem.getFieldName());
                //以UTF-8字符集获取该表项的字符串
                System.out.println("表单的内容"+fileItem.getString("UTF-8"));
            }else{
                //是文件类型
                System.out.println("表单的属性值"+fileItem.getFieldName());
                System.out.println("上传的文件名"+fileItem.getName());
                fileItem.write(new File("e:\\"+fileItem.getName()));
            }
        }
    } catch (Exception e) {
        e.printStackTrace();
    }
}
```

jsp中：

```jsp
<body>
    <form action="http://localhost:8080/jsp/uploadServlet" method="post" enctype="multipart/form-data" >
        username:<input type="text" name="username"><br>
        头像<input type="file" name="photo"><br>
        <input type="submit" value="上传">
    </form>
</body>
```

### 2.文件下载

使用包commons-fileupload和commons-io

```java
public class Download extends HttpServlet {
    @Override
    protected void doGet(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        //1.获取要下载的文件名
        String downloadFileName = "胡桃.jpg";
        //2.读取要下载的文件内容
            //需要一个ServletContext对象来获取文件的信息
        ServletContext servletContext = getServletContext();

            //提供该文件在工程下的路径以获取文件的类型
        String mimeType = servletContext.getMimeType("/photo/" + downloadFileName);
        System.out.println("要下载的文件类型是"+mimeType);
        //3.通过设置响应头的方式，在回传给客户端前，告知客户端要返回的文件类型
        resp.setContentType(mimeType);
        //4.设置响应头告知客户端收到的数据是供客户端下载使用
            //注意字段Content而不是Context
            //Content-Disposition:(内容—支配)表示对数据的处理方式的响应头
            //attachment:附件，表示下载
            //filename指定下载名
        resp.setHeader("Content-Disposition","attachment;filename="+downloadFileName);
        //5.将输入流的数据复制给输出流，传输给客户端，传输一定要放在设置响应头之后
            //从servletContext填入资源路径获取输入流
        InputStream inputStream = servletContext.getResourceAsStream("/photo/" + downloadFileName);
            //从resp获取输出流
        ServletOutputStream outputStream = resp.getOutputStream();
        IOUtils.copy(inputStream,outputStream);
    }
}
```

解决字符集导致的乱码的问题：

在第4步设置字符集

```
String str = "attachment;filename="+URLEncoder.encode("文件的名字", "UTF-8");
resp.setHeader("Content-Disposition",str);
```

