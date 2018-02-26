#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>

#define DEFAULT_BACKLOG 5
#define BUFFER_SIZE 256

volatile sig_atomic_t sig_int = 0 ;

void sig_handle ( int sig_num )
{
  if ( sig_num == SIGINT )
  {
   sig_int = 1 ;
  }
} 

void error ( const char * msg )
{
  perror ( msg ) ;
  exit ( 0 ) ;
}

int main ( int argc , char * argv [] )
{
  int fd_socket , fd_socket_new , bind_return , listen_port ;
  struct sockaddr_in my_sock , their_sock ;
  int buff [ 1 ]  ;
  char * msg [ BUFFER_SIZE ] ;

  struct sigaction sig_act ;
  memset ( & sig_act , 0 , sizeof ( struct sigaction ) ) ;
  sig_act .sa_handler = sig_handle ;
  sigaction ( SIGINT , & sig_act , NULL ) ;

  memset ( msg , 0 , BUFFER_SIZE ) ;
  buff [ 0 ] = 0 ;

  if ( argc < 2 )
  {
    fprintf ( stderr , "ERROR, you must enter a port number as an argument\n" ) ;
    exit ( 1 ) ;
  }

  fd_socket = socket ( AF_INET , SOCK_STREAM , IPPROTO_TCP ) ;
  if ( fd_socket < 0 ) error ( "ERROR, cannot open socket" ) ;


  memset ( & my_sock , 0 , sizeof ( struct sockaddr ) ) ;

  my_sock .sin_family = AF_INET ;
  my_sock .sin_port = htons ( atoi ( argv [ 1 ] ) ) ;
  my_sock .sin_addr .s_addr = INADDR_ANY ;


  bind_return = bind ( fd_socket , & my_sock , sizeof ( my_sock ) ) ;
  if ( bind_return < 0 ) error ( "ERROR, cannot bind socket" ) ;

  listen ( fd_socket , DEFAULT_BACKLOG ) ;

  int their_size = sizeof ( their_sock ) ;

  while ( sig_int != 1 )
  {
    fd_socket_new = accept ( fd_socket , & their_sock , & their_size ) ;
    //if ( fd_socket_new < 0 ) error ( "ERROR, cannot accept connection" ) ;

    int read_int_success ;

    read_int_success = read ( fd_socket_new , buff , sizeof ( int ) ) ;
    //if ( read_int_success != 0 ) error ( "ERROR, could not recieve from client" ) ;

    int reply ;
    reply = * buff + 2 ;

    int reply_success ;
    reply_success = write ( fd_socket_new , & reply , sizeof ( int ) ) ;
    //if ( reply_success != 0 ) error ( "ERROR , could not send reply to client" ) ;
  }

  printf ( "\nKilling server...\n" ) ;

  close ( fd_socket_new ) ;
  close ( fd_socket ) ;

  return 0 ;
}
