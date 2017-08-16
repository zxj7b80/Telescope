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
		//1.����һ����������Socket����ServerSocket��ָ���󶨵Ķ˿ڣ��������˶˿�
		ServerSocket serverSocket = new ServerSocket(5001);
		InetAddress address = InetAddress.getLocalHost();
		String ip = address.getHostAddress();
		Socket socket = null;
		//2.����accept()�ȴ��ͻ�������
		System.out.println("~~~������Ѿ������ȴ��ͻ��˽���~�������ip��ַ: " + ip);
		socket = serverSocket.accept();
		System.out.println("�ͻ����Ѿ��ɹ����룡");
		//3.���Ӻ��ȡ����������ȡ�ͻ�����Ϣ
		InputStream is=null;
		InputStreamReader isr=null;
		BufferedReader br=null;
		OutputStream os=null;
		PrintWriter pw=null;
		is = socket.getInputStream();     //��ȡ������
		isr = new InputStreamReader(is,"UTF-8");
		br = new BufferedReader(isr);
		String info = null;
		String info_ok = null;
		
		while((info=br.readLine())!=null){//ѭ����ȡ�ͻ��˵���Ϣ
			System.out.println("�ͻ��˷��͹�������Ϣ" + info);
			info_ok = info;
		}
		socket.shutdownInput();//�ر�������
		socket.close();
		  
		//����TCP�ͻ���
        System.out.println("Connecting to " + serverName + " on port " + port);
        Socket client = new Socket(serverName, port);
		//���
		OutputStream outToServer = client.getOutputStream();
        DataOutputStream out = new DataOutputStream(outToServer);
		//method1---
		//String temp = new String("<CmdCUID=2.9.0.1.1><Jog=I1702>");//�������ֶ����У��������ģ�//info;//new String(info);//"<CmdCUID=2.9.0.1.1><Jog=#17j+>"
        //out.write(temp.getBytes("UTF-8"));  
		//method2---
		out.write(info_ok.getBytes("UTF-8"));
		System.out.println("Messages have been sent!");
		//����
		InputStream inFromServer = client.getInputStream();
		DataInputStream in = new DataInputStream(inFromServer);
		byte[] by = new byte[1024];
		in.read(by);
		String str1 = new String(by);
		System.out.println("Get some messages: " + str1);
		//�ر�
		client.close();
      }catch(IOException e)
      {
		  e.printStackTrace();
      }
   }
}