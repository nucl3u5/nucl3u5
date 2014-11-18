/*
# Intro 
本プログラムはSSHサービスの接続可否を調査し、接続可能であれば許容されている認証メソッドを取得し表示します。 ssh_scanはIPアドレスとポート番号をパラメータとして受け取ります。
# Dependence 
 * libssh-4 - tiny C SSH library
# See Also 
 * libssh-doc/html/libssh_tutorial.html
 * http://www.geeksforgeeks.org/program-to-validate-an-ip-address/
# Contributor 
 * nucleus@cyberwarfare.xyz
*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <libssh/libssh.h>

void usage(){
   printf("ssh_scan -h [ip_addr|hostname] -p [port_num]\n");
   exit(1);
}

int main(int argc, char *argv[]){
   char ch;
   char *host_param = NULL; 
   char *port_param = NULL;
   int timeout = 5;

   /* オプション解析 */
   while ((ch = getopt(argc, argv, "h:p:")) != EOF){
      switch(ch) {
         case 'h':
            host_param = optarg;
            break;
         case 'p':
            port_param = optarg;
            break;
      }
   }
   argc -= optind;
   argv += optind;

   /* SSH SESSION 開始 */
   ssh_session my_ssh_session;
   char *host;
   int port;
//   int verbosity = SSH_LOG_PROTOCOL;
//   int verbosity = SSH_LOG_PACKET;
   int verbosity = SSH_LOG_NOLOG;
   int rc;
   int method;

   if(host_param==NULL){
      host = "localhost";
   }else{
      host = host_param;
   }
   
   if(port_param==NULL){
      port = 22;
   }else{
      port = atoi(port_param);
   }

   my_ssh_session = ssh_new();
   if (my_ssh_session == NULL)
      exit(-1);

   printf("%s:%d\n", host, port);

   ssh_options_set(my_ssh_session, SSH_OPTIONS_HOST, host);
   ssh_options_set(my_ssh_session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
   ssh_options_set(my_ssh_session, SSH_OPTIONS_PORT, &port);
   ssh_options_set(my_ssh_session, SSH_OPTIONS_TIMEOUT, &timeout);

   rc = ssh_connect(my_ssh_session);
   if(rc != SSH_OK){
      fprintf(stderr, "Error connecting to localhost: %s\n", 
               ssh_get_error(my_ssh_session));
      ssh_free(my_ssh_session);
      //コネクションが張れないのは正常終了であることに注意
      exit(0);
   } 

   method = ssh_auth_list(my_ssh_session); 

   if(method & SSH_AUTH_METHOD_PASSWORD){
      printf("   * SSH_AUTH_METHOD_PASSWORD\n");
   }
   if(method & SSH_AUTH_METHOD_PUBLICKEY){
      printf("   * SSH_AUTH_METHOD_PUBLICKEY\n");
   }
   if(method & SSH_AUTH_METHOD_HOSTBASED){
      printf("   * SSH_AUTH_METHOD_HOSTBASED\n");
   }
   if(method & SSH_AUTH_METHOD_INTERACTIVE){
      printf("   * SSH_AUTH_METHOD_INTERACTIVE\n");
   } 

   ssh_disconnect(my_ssh_session);
   ssh_free(my_ssh_session);

   // ここまで来るなら報告すべき
   exit(1);
}


