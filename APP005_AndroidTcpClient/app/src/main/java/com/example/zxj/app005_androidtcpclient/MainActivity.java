package com.example.zxj.app005_androidtcpclient;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.io.IOException;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;

public class MainActivity extends AppCompatActivity {

    private TextView name;
    private EditText editIP;
    private EditText editPORT;
    private Button btn_accept;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        name = (TextView)findViewById(R.id.name);
        editIP = (EditText)findViewById(R.id.editIP);
        editPORT = (EditText)findViewById(R.id.editPORT);

        btn_accept = (Button)findViewById(R.id.btn_accept);
        btn_accept.setOnClickListener(new View.OnClickListener() {
            //重写点击事件的处理方法onClick()
            @Override
            public void onClick(View v) {
                name.setText("connecting...");
                final String ip = editIP.getText().toString();
                final int port = 5001;
//                port = editPORT.getText().toString();
                new Thread() {
                    @Override
                    public void run() {
                        try {
                            acceptServer(ip, port);
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                }.start();
            }
        });
    }

    private void acceptServer(String IP,int Port) throws IOException {
        //1.创建客户端Socket，指定服务器地址和端口
        Socket socket = new Socket(IP, Port);
        //2.获取输出流，向服务器端发送信息
        OutputStream os = socket.getOutputStream();//字节输出流
        //method1----------------
//        PrintWriter pw = new PrintWriter(os);//将输出流包装为打印流
//        //发送客户端的IP地址
//        pw.write("客户端：~" + socket.getLocalAddress() + "~ 接入服务器！！");
//        pw.flush();
//        socket.shutdownOutput();//关闭输出流
        //method2-----------------
        String temp = new String("<CmdCUID=2.9.0.1.1><Jog=I1702>");
        os.write(temp.getBytes("UTF-8"));
        socket.close();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}
