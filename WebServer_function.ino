
String postForms(){
  String html = "<html>\
  <head>\
    <title>图像管理页面</title>\
    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\">\
    <meta name='viewport' content='width=device-width, initial-scale=1.0, user-scalable=no, minimum-scale=1.0, maximum-scale=1.0'/>\
    <style>\
      body { font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>图像管理页面</h1><br>\
    <form method=\"post\" enctype=\"multipart/form-data\" action=\"/postform/\">\
      <input type=\"file\" name=\"name\" >\
      <input type=\"submit\" value=\"上传\">\
    </form>";
    
  for(int i=0;i<filename_num;i++){
    html += "<img src=\"/";
    html += Fname[i];
    html += "\"/>";
    
    html += "<p>";
    html += Fname[i];
    html += "<form method=\"post\" enctype=\"application/x-www-form-urlencoded\" action=\"/postdel/\" accept-charset=\"UTF-8\">";
    html += "<input type=\"submit\" name=\"";
    html += Fname[i];
    html += "\" value=\"删除\">";
    html += "</form>";
    html += "</p>";
    
  }
  html +="</body></html>";
     
  return html;
}


void handleRoot() {
  listDir("/");
  server.send(200, "text/html", postForms());
  
}

void handleForm() {
  HTTPUpload& upload = server.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/"))filename = "/" + filename;
    Serial.println("File Name: " + filename);  
    uploadFile = LittleFS.open(filename, "w"); 
    
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    
    if (uploadFile)uploadFile.write(upload.buf, upload.currentSize);
    
    Serial.print(".");  
    
  } else if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile) {
      Serial.println("UPLOAD_FILE_END"); 
      uploadFile.close();
      Serial.println(" ");  
      Serial.println("ok"); 
      delay(500);
      listDir("/");
      server.send(200, "text/html", postForms());
      select_num = 0;
      returnImg();
      
    }
  }
   // listFiles();
    
}

// 处理用户浏览器的HTTP访问
void handleUserRequest(){
                              
  // 获取用户请求网址信息
  String webAddress = server.uri();
  
  // 通过handleFileRead函数处处理用户访问
  bool fileReadOK = handleFileRead(webAddress);

  // 如果在SPIFFS无法找到用户访问的资源，则回复404 (Not Found)
  if (!fileReadOK){                                                 
    server.send(404, "text/plain", "404 Not Found"); 
  }
}

bool handleFileRead(String path) {            //处理浏览器HTTP访问

  String contentType = getContentType(path);  // 获取文件类型
  
  if (LittleFS.exists(path)) {                     // 如果访问的文件可以在SPIFFS中找到
    //File file = SPIFFS.open(path, "r");          // 则尝试打开该文件
    File file = LittleFS.open(path, "r");
    server.streamFile(file, contentType);// 并且将该文件返回给浏览器
    file.close();                                // 并且关闭文件
    return true;                                 // 返回true
  }
  return false;                                  // 如果文件未找到，则返回false
}

// 获取文件类型
String getContentType(String filename){
  if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void respondOK(){
  server.send(200);
  
}

void handledel(){
  String message = "";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  Serial.println(server.argName(0));
  String delfile = server.argName(0);
  LittleFS.remove(delfile);
  listDir("/");
  server.send(200, "text/html", postForms());
  
}
