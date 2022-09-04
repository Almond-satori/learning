

# JDBC

## 1.获取连接

### 1.原始方式

```java
public class jdbc1 {
    public static void main(String[] args) throws SQLException {
        //新建驱动对象,注意这里写了第三方的jar包，因为Driver是不确定的，可能是oracle，mysql的Driver等等
        Driver driver = new com.mysql.jdbc.Driver();
        //提供驱动所需的一些配置
        String url = "jdbc:mysql://localhost:3306/test";
        //jdbc:mysql 协议
        //localhost:3306 ip:port
        // test指的是访问数据库test
        //将用户名和密码封装在properties
        Properties properties = new Properties();

        //这里的user和password是写死的不能成写username
        properties.setProperty("user","root");
        properties.setProperty("password","932723");

        Connection conn = driver.connect(url, properties);
        System.out.println(conn);
    }
}
```

### 2.使用DriverManager管理Driver的方式获取连接

一般我们习惯使用DriverManager管理Driver的方式获取连接：

```java
public static void main(String[] args) throws Exception {
        //准备数据
        String url = "jdbc:mysql://localhost:3306/test";
        String user = "root";
        String password = "932723";
        String driverClass ="com.mysql.jdbc.Driver";

        //我们需要使用不同的数据库，就要使用不同的驱动，所以将这个驱动jar包作为变量抽出来作为driverClass
        Class clazz = Class.forName(driverClass);
        //创建驱动的实例
        Driver driver = (Driver) clazz.newInstance();
        //向驱动管理注册驱动
        DriverManager.registerDriver(driver);
        //获取连接
        Connection conn = DriverManager.getConnection(url, user, password);
        System.out.println(conn);
}
```

### 3.最终方式

在src文件下创建jdbc.properties，写入：

```
user=root
password=932723
url=jdbc:mysql://localhost:3306/test
driverClass=com.mysql.jdbc.Driver
```

在java文件中对jdbc.properties文件进行读取，要注意的是，Class.forName处，我们实际上实现了向DriverManager注册的操作：

```java
public class jdbc5 {
    public static void main(String[] args) throws Exception {
        //类的加载器可以用来加载流，相当于输入流，我们目前是要加载jdbc的配置文件
        InputStream is = jdbc5.class.getClassLoader().getResourceAsStream("jdbc.properties");
        //创建Properties类
        Properties pro = new Properties();
        //pro加载配置文件
        pro.load(is);
        //获取需要的信息
        String user = pro.getProperty("user");
        String password = pro.getProperty("password");
        String url = pro.getProperty("url");
        String driverClass = pro.getProperty("driverClass");

        //加载类到内存，（这意味着静态代码块被等被执行,而com.mysql.jdbc.Driver的静态代码块中帮我们向driverManager注册了driver）:
        //    static {
        //        try {
        //            DriverManager.registerDriver(new Driver());
        //        } catch (SQLException var1) {
        //            throw new RuntimeException("Can't register driver!");
        //        }
        //    }
        Class.forName(driverClass);
        //获取连接
        Connection conn = DriverManager.getConnection(url, user, password);
        System.out.println(conn);
    }
}
```

## 2.CRUD

### 1.Statement

注意在java中区分大小写，而mysql中不区分。我们在java中写sql语句时，查询的字段名应该使用对应的类中的参数名。

```java
// 使用Statement的弊端：需要拼写sql语句，并且存在SQL注入的问题
public static void main(String[] args) {
    //输入username和密码
    Scanner scanner = new Scanner(System.in);
    System.out.println("username:");
    String username = scanner.next();
    System.out.println("password:");
    String password = scanner.next();

    //注意在sqlyog中会自动大写，但是我们写的User类中的参数是小写的所以要改为小写的user和password!
    String sql = "SELECT user,password FROM user_table WHERE user='" +
            username + "' AND password='" + password + "';";
    
    StatementTest s = new StatementTest();
    User user = s.get(sql, User.class);
    //如果数据库没有这个值，返回null
    if (user != null) {
        System.out.println("登录成功");
    } else {
        System.out.println("登录失败");
    }
}
```

sql注入：

```
username:
1' OR 
password:
=1 OR '1' = '1
登录成功

拼接成的sql语句是
SELECT USER,PASSWORD FROM user_table 
WHERE USER='1' OR '  AND password = '=1 OR '1' = '1';

最后一个or是恒成立的所以能登录成功
```

### 2.PreparedStatement

#### 1.预编译执行sql语句

为了方便看过程没有使用try-catch的代码：

```java
public class PreparedStatementTest {
    public static void main(String[] args) throws Exception {
        //获取连接
        Connection conn = JDBCUtils.getConnection();
        //预编译sql语句
        String sql ="insert into customers(name,email,birth) values(?,?,?)";
        PreparedStatement ps = conn.prepareStatement(sql);
        //填充占位符，注意占位符索引从1开始
        ps.setString(1,"tom");
        ps.setString(2,"tom@qq.com");
        SimpleDateFormat sdf= new SimpleDateFormat("yyyy-MM-dd");
        java.util.Date date = sdf.parse("2001-3-4");
        ps.setDate(3,new java.sql.Date(date.getTime()));
        //执行sql
        ps.execute();
        //关闭资源
        JDBCUtils.closeResource(conn,ps);
    }
}
```

JDBCUtils:

```java
    public static Connection getConnection() throws IOException, ClassNotFoundException, SQLException {
        //类的加载器可以用来加载流，相当于输入流，我们目前是要加载jdbc的配置文件
        InputStream is = ClassLoader.getSystemResourceAsStream("jdbc.properties");
        //创建Properties类
        Properties pro = new Properties();
        //pro加载配置文件
        pro.load(is);
        //获取需要的信息
        String user = pro.getProperty("user");
        String password = pro.getProperty("password");
        String url = pro.getProperty("url");
        String driverClass = pro.getProperty("driverClass");
        //获取连接
        Class.forName(driverClass);
        Connection conn = DriverManager.getConnection(url, user, password);
        return conn;
    }

    public static void closeResource(Connection conn, Statement ps){
        try {
            if (ps!=null)
                ps.close();
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        try {
            if(conn!=null)
                conn.close();
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
    }
```

使用try-catch的代码：

```java
public class PreparedStatementTest {
    public static void main(String[] args) throws Exception {
        Connection conn = null;
        PreparedStatement ps = null;
        try {
            //1.获取连接
            conn = JDBCUtils.getConnection();
            //2.预编译sql语句
            String sql ="insert into customers(name,email,birth) values(?,?,?)";
            ps = conn.prepareStatement(sql);
            //3.填充占位符，注意占位符索引从1开始
            ps.setString(1,"tom");
            ps.setString(2,"tom@qq.com");
            SimpleDateFormat sdf= new SimpleDateFormat("yyyy-MM-dd");
            Date date = sdf.parse("2001-3-4");
            ps.setDate(3,new java.sql.Date(date.getTime()));
            //4.执行sql
            ps.execute();
        } catch (Exception e) {
            e.printStackTrace();
        }finally {
            //5.关闭资源
            JDBCUtils.closeResource(conn,ps);
        }
    }
}
```



#### 2.通用的增删改操作

```java
//通用的增删改操作
public void update(String sql,Object ...args){
    Connection conn = null;
    PreparedStatement ps = null;
    try {
        //获取连接
        conn = JDBCUtils.getConnection();
        //预编译sql语句
        ps = conn.prepareStatement(sql);
        //填充占位符
        for (int i = 0;i<args.length;i++){
            //注意占位符下标从1开始
            ps.setObject(i+1,args[i]);
        }
        //执行
        //想要返回是否操作成功使用:
        //ps.excuteUpdate()可以返回影响的行数，若操作失败返回为0
        ps.execute();
       
    } catch (Exception e) {
        e.printStackTrace();
    } finally {
        //关闭资源
        JDBCUtils.closeResource(conn,ps);
    }
}
```

#### 3.通用的查询操作

##### (1)一个针对特定表的查询，使用时改为try-catch处理异常：

```java
public static void main(String[] args) throws Exception {

    Connection conn = JDBCUtils.getConnection();
    String sql ="select id,name,email,birth from customers where id = ?";
    PreparedStatement ps = conn.prepareStatement(sql);
    ps.setObject(1,1);
    //使用执行查询语句返回一个结果集
    ResultSet rs = ps.executeQuery();
    //因为知道只有一条数据，所以用if
    if (rs.next()){
        int id = rs.getInt(1);
        String name = rs.getString(2);
        String email = rs.getString(3);
        Date birth = rs.getDate(4);
    }
    JDBCUtils.closeResource(conn,ps,rs);
}
```



##### (2)针对customers表的通用查询操作：（注意改为try-catch处理）

```java
/**
 * 该方法返回一个Customer对象，意味着查询结果集必须是只有一行的
 * @param sql
 * @param args
 * @return
 * @throws Exception
 */
public static Customer getCustomer(String sql,Object ...args) throws Exception {
    Connection conn = JDBCUtils.getConnection();
    PreparedStatement ps = conn.prepareStatement(sql);
    for (int i = 0;i< args.length;i++){
        ps.setObject(i+1,args[i]);
    }
    ResultSet rs = ps.executeQuery();

    //返回一个结果集的元数据，什么是元数据？
    //元数据是指描述数据的数据，比如int id = 10，int和id都是描述数据10的，是10的元数据。
    //在这里是指描述结果集的数据，比如我们需要获得的列数
    ResultSetMetaData rsmd = rs.getMetaData();
    //获取查询的列数
    int columnCount = rsmd.getColumnCount();
    if(rs.next()){
        Customer cust = new Customer();
        for (int i = 0;i<columnCount;i++){
            //获取列值
            Object columnValue = rs.getObject(i + 1);
            //获取列名
            //getColumnName 不推荐使用，因为当表中字段和java中类的属性值不相同时，我们通过在sql中取别名的方式解决这一问题
            //getColumnName只能拿到列的值而getColumnLabel可以拿到别名,没有别名就拿到列名
            String conlumnName = rsmd.getColumnLabel(i + 1);
            //动态地给属性赋值
                //获取域
            Field field = Customer.class.getDeclaredField(conlumnName);
                //属性值为private，需要设置访问权限
            field.setAccessible(true);
            field.set(cust,columnValue);
        }
        return cust;
    }
    JDBCUtils.closeResource(conn,ps,rs);
    return null;
}
```

##### (3)对于任何表的查询(Final)



可查询任何表的通用写法：（返回值为单个数据）

```java
public static <T> T getInstance(Class<T> clazz,String sql,Object ...args){
    Connection conn = null;
    PreparedStatement ps = null;
    ResultSet rs = null;
    try {
        conn = JDBCUtils.getConnection();
        ps = conn.prepareStatement(sql);
        for (int i = 0;i< args.length;i++){
            ps.setObject(i+1,args[i]);
        }
        rs = ps.executeQuery();

        //返回一个结果集的元数据，什么是元数据？
        //元数据是指描述数据的数据，比如int id = 10，int和id都是描述数据10的，是10的元数据。
        //在这里是指描述结果集的数据，比如我们需要获得的列数
        ResultSetMetaData rsmd = rs.getMetaData();
        //获取查询的列数
        int columnCount = rsmd.getColumnCount();
        if(rs.next()){
            T t = clazz.newInstance();
            for (int i = 0;i<columnCount;i++){
                //获取列值
                Object columnValue = rs.getObject(i + 1);
                //获取列名
                //getColumnName 不推荐使用，因为当表中字段和java中类的属性值不相同时，我们通过在sql中取别名的方式解决这一问题
                //getColumnName只能拿到列的值而getColumnLabel可以拿到别名,没有别名就拿到列名
                String columnName = rsmd.getColumnLabel(i + 1);
                //动态地给属性赋值
                //获取域
                Field field = Customer.class.getDeclaredField(columnName);
                //属性值为private，需要设置访问权限
                field.setAccessible(true);
                field.set(t,columnValue);
            }
            return t;
        }
    } catch (Exception e) {
        e.printStackTrace();
    } finally {
        JDBCUtils.closeResource(conn,ps,rs);
    }
    return null;
}
```



多条查询结果：

```java
public static <T> List<T> getInstances(Class<T> clazz, String sql, Object ...args){
    Connection conn = null;
    PreparedStatement ps = null;
    ResultSet rs = null;
    try {
        conn = JDBCUtils.getConnection();
        ps = conn.prepareStatement(sql);
        for (int i = 0;i< args.length;i++){
            ps.setObject(i+1,args[i]);
        }
        rs = ps.executeQuery();

        //返回一个结果集的元数据，什么是元数据？
        //元数据是指描述数据的数据，比如int id = 10，int和id都是描述数据10的，是10的元数据。
        //在这里是指描述结果集的数据，比如我们需要获得的列数
        ResultSetMetaData rsmd = rs.getMetaData();
        //获取查询的列数
        int columnCount = rsmd.getColumnCount();
        ArrayList<T> list = new ArrayList();
        while(rs.next()){
            T t = clazz.newInstance();
            for (int i = 0;i<columnCount;i++){
                //获取列值
                Object columnValue = rs.getObject(i + 1);
                //获取列名
                //getColumnName 不推荐使用，因为当表中字段和java中类的属性值不相同时，我们通过在sql中取别名的方式解决这一问题
                //getColumnName只能拿到列的值而getColumnLabel可以拿到别名,没有别名就拿到列名
                String columnName = rsmd.getColumnLabel(i + 1);
                //动态地给属性赋值
                //获取域
                Field field = Customer.class.getDeclaredField(columnName);
                //属性值为private，需要设置访问权限
                field.setAccessible(true);
                field.set(t,columnValue);
            }
            list.add(t);
        }
        return list;
    } catch (Exception e) {
        e.printStackTrace();
    } finally {
        JDBCUtils.closeResource(conn,ps,rs);
    }
    return null;
}
```

### 3.Blob

向数据库中写入Blob（图片，视频等）:

```java
Connection conn = JDBCUtils.getConnection();
String sql = "insert into customers(name,email,birth,photo) values(?,?,?,?)";

PreparedStatement ps = conn.prepareStatement(sql);
ps.setObject(1,"HuTao");
ps.setObject(2,"hutao@almond.com");
ps.setObject(3,"2001-08-07");

//创建文件，注意文件路径写法
File file = new File("src/hutao.jpg");
FileInputStream fis = new FileInputStream(file);
//文件的输入流可以作为参数传入ps
ps.setObject(4,fis);
ps.execute();

JDBCUtils.closeResource(conn,ps);
fis.close();
```



从数据库查询Blob文件并输出：

```java
@Test
public void testOutputBlob() throws Exception {
    Connection conn = null;
    PreparedStatement ps = null;
    ResultSet rs = null;
    InputStream is = null;
    FileOutputStream fos = null;
    try {
        conn = JDBCUtils.getConnection();
        String sql ="select photo from customers where id =?";
        ps = conn.prepareStatement(sql);
        ps.setObject(1,21);

        rs = ps.executeQuery();
        if(rs.next()){
            //可以采用写label（字段的代替值）的方式获取结果
            Blob photo = rs.getBlob("photo");
            //建立输入流读取Blob，建立输出流输出数据库中的Blob
            is = photo.getBinaryStream();
            fos = new FileOutputStream("src/hutaoRe.jpg");
            //数据从输入流传输到输出流
            byte[] buffer = new byte[1024];
            int len;
            while( (len=is.read(buffer))!= -1 ){
                fos.write(buffer,0,len);
            }
        }
    } catch (Exception e) {
        e.printStackTrace();
    } finally {
        try {
            if (is!=null)
                is.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        try {
            if (fos!=null)
                fos.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        JDBCUtils.closeResource(conn,ps,rs);
    }
}
```

### 4.批量操作数据

一条一条的插入方式

```java
public static void main(String[] args) throws Exception {
    Connection conn = null;
    PreparedStatement ps = null;
    try {
        conn = JDBCUtils.getConnection();
        String sql = "insert into goods(name) values(?)";
        ps = conn.prepareStatement(sql);
        long start = System.currentTimeMillis();
        for (int i = 1;i<=20000;i++){
            ps.setObject(1,"name_"+i);
            ps.execute();
        }
        long end = System.currentTimeMillis();
        System.out.println(end-start);//27563
    } catch (IOException e) {
        e.printStackTrace();
    } finally {
        JDBCUtils.closeResource(conn,ps);
    }
}
```

批量插入方式：

首先在jdbc.properties中修改url

```properties
url=jdbc:mysql://localhost:3306/test?rewriteBatchedStatements=true
```

并且更换为mysql 5.1.37的jar包

进一步优化：设置自动提交为false，在完成操作后手动提交

## 3.事务

>数据库的自动提交
>
>1.mysql默认在每次sql语句执行后都进行一次提交
>
>2.在数据库连接关闭时会进行提交

解决方法：

首先在连接中禁用自动提交：

```java
conn.setAutoCommit(false);//conn是连接的对象

conn.commit()//采用手动提交
conn.rollback()//出现异常手动回滚
```

对于连接，每换一次连接，上述设置就没用了，而且连接关闭会进行提交操作

**我们需要将一次事务在一次连接中完成**，修改通用update和通用查询方法：

将连接从外部传入，并且在关闭资源时，不关闭连接

```java
//通用的增删改操作
public static void update(Connection conn, String sql, Object... args){
    PreparedStatement ps = null;
    try {
        //预编译sql语句
        ps = conn.prepareStatement(sql);
        //填充占位符
        for (int i = 0;i<args.length;i++){
            //注意占位符下标从1开始
            ps.setObject(i+1,args[i]);
        }
        //执行
        ps.execute();
    } catch (Exception e) {
        e.printStackTrace();
    } finally {
        //关闭资源
        try {
            //将连接恢复原样(针对连接池)
            conn.setAutoCommit(true);
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        JDBCUtils.closeResource(null,ps);
    }
}

/*---------------------------------------------------------------------------------------*/
//通用的查询操作
public static <T> T getInstance(Connection conn,Class<T> clazz,String sql,Object ...args){
        PreparedStatement ps = null;
        ResultSet rs = null;
        try {
            ps = conn.prepareStatement(sql);
            for (int i = 0;i< args.length;i++){
                ps.setObject(i+1,args[i]);
            }
            rs = ps.executeQuery();

            //返回一个结果集的元数据，什么是元数据？
            //元数据是指描述数据的数据，比如int id = 10，int和id都是描述数据10的，是10的元数据。
            //在这里是指描述结果集的数据，比如我们需要获得的列数
            ResultSetMetaData rsmd = rs.getMetaData();
            //获取查询的列数
            int columnCount = rsmd.getColumnCount();
            if(rs.next()){
                T t = clazz.newInstance();
                for (int i = 0;i<columnCount;i++){
                    //获取列值
                    Object columnValue = rs.getObject(i + 1);
                    //获取列名
                    //getColumnName 不推荐使用，因为当表中字段和java中类的属性值不相同时，我们通过在sql中取别名的方式解决这一问题
                    //getColumnName只能拿到列的值而getColumnLabel可以拿到别名,没有别名就拿到列名
                    String columnName = rsmd.getColumnLabel(i + 1);
                    //动态地给属性赋值
                    //获取域
                    Field field = Customer.class.getDeclaredField(columnName);
                    //属性值为private，需要设置访问权限
                    field.setAccessible(true);
                    field.set(t,columnValue);
                }
                return t;
            }
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            JDBCUtils.closeResource(null,ps,rs);
        } 
        return null;
    }
```
