import java.net.*;
import java.io.*;
 
public class Medium
{
   public static void main(String [] args)
   {
      String serverName = args[0];
      int port = Integer.parseInt(args[1]);
      try
      {
		//1.创建一个服务器端Socket，即ServerSocket，指定绑定的端口，并监听此端口
		ServerSocket serverSocket = new ServerSocket(5001);
		InetAddress address = InetAddress.getLocalHost();
		String ip = address.getHostAddress();
		Socket socket = null;
		//2.调用accept()等待客户端连接
		System.out.println("~~~服务端已就绪，等待客户端接入~，服务端ip地址: " + ip);
		socket = serverSocket.accept();
		System.out.println("客户端已经成功接入！");
		//3.连接后获取输入流，读取客户端信息
		InputStream is=null;
		InputStreamReader isr=null;
		BufferedReader br=null;
		OutputStream os=null;
		PrintWriter pw=null;
		is = socket.getInputStream();     //获取输入流
		isr = new InputStreamReader(is,"UTF-8");
		br = new BufferedReader(isr);
		String info = null;
		String info_ok = null;
		
		while((info=br.readLine())!=null){//循环读取客户端的信息
			System.out.println("客户端发送过来的信息" + info);
			info_ok = info;
		}
		socket.shutdownInput();//关闭输入流
		socket.close();
		  
		//建立TCP客户端
        System.out.println("Connecting to " + serverName + " on port " + port);
        Socket client = new Socket(serverName, port);
		//输出
		OutputStream outToServer = client.getOutputStream();
        DataOutputStream out = new DataOutputStream(outToServer);
		//method1---
		//String temp = new String("<CmdCUID=2.9.0.1.1><Jog=I1702>");//后面两种都不行，问题在哪？//info;//new String(info);//"<CmdCUID=2.9.0.1.1><Jog=#17j+>"
        //out.write(temp.getBytes("UTF-8"));  
		//method2---
		out.write(info_ok.getBytes("UTF-8"));
		System.out.println("Messages have been sent!");
		//输入
		InputStream inFromServer = client.getInputStream();
		DataInputStream in = new DataInputStream(inFromServer);
		byte[] by = new byte[1024];
		in.read(by);
		String str1 = new String(by);
		System.out.println("Get some messages: " + str1);
		//关闭
		client.close();
      }catch(IOException e)
      {
		  e.printStackTrace();
      }
   }
}