exception Ssh_error

let () =
  let session = Ssh.create_session () in
  Ssh.set_host session "example.com" ;
  Ssh.set_port session 22 ;
  Ssh.set_user session "user" ;
  if not (Ssh.connect session) then raise (Failure "Bad host") ;
  if not (Ssh.auth_pass session "p4ssw0rd") then (
    Ssh.disconnect session ;
    raise (Failure "Bad password") ) ;
  let response = Ssh.execs session "uname -a" in
  print_endline (String.trim response) ;
  Ssh.disconnect session
