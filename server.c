#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>

#define DEFAULT_BACKLOG 5

void error ( const char * msg )
{
  perror ( msg ) ;
  exit ( 0 ) ;
}

int main ( int argc , char * argv [] )
{
  int fd_socket , fd_socket_new , bind_return , listen_port ;

  if ( argc < 2 )
  {
    fprintf ( stderr , "ERROR, you must enter a port number as an argument\n" ) ;
    exit ( 1 ) ;
  }

  fd_socket = socket ( AF_INET , SOCK_STREAM , IPPROTO_TCP ) ;
  if ( fd_socket < 0 ) error ( "ERROR, cannot open socket" ) ;


  struct sockaddr_in my_sock , their_sock ;
  memset ( & my_sock , 0 , sizeof ( struct sockaddr ) ) ;

  my_sock .sin_family = AF_INET ;
  my_sock .sin_port = htons ( atoi ( argv [ 1 ] ) ) ;
  my_sock .sin_addr .s_addr = INADDR_ANY ;


  bind_return = bind ( fd_socket , & my_sock , sizeof ( my_sock ) ) ;
  if ( bind_return < 0 ) error ( "ERROR, cannot bind socket" ) ;

  listen ( fd_socket , DEFAULT_BACKLOG ) ;

  int their_size = sizeof ( their_sock ) ;

  fd_socket_new = accept ( fd_socket , & their_sock , & their_size ) ;
  if ( fd_socket_new < 0 ) error ( "ERROR, cannot accept connection" ) ;


   

  return 0 ;
}
