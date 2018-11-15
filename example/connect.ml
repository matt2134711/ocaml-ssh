let () =
  let session = Ssh.create_session () in
  Ssh.set_host session "example.com" ;
  Ssh.set_port session 22 ;
  Ssh.set_user session "user" ;
  if Ssh.connect session && Ssh.auth_pass session "p4ssw0rd" then (
    Ssh.disconnect session ; print_endline "It works!" )
  else print_endline "Server is offline or incorrect password"
