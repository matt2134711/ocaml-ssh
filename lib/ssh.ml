type channel

type session

external auth_pass : session -> string -> bool = "caml_ssh_auth_pass"

external connect : session -> bool = "caml_ssh_connect"

external close_channel : channel -> unit = "caml_ssh_close_channel"

external create_channel : session -> channel = "caml_ssh_create_channel"

external create_session : unit -> session = "caml_ssh_create_session"

external disconnect : session -> unit = "caml_ssh_disconnect"

external read : channel -> string = "caml_ssh_read"

external request_exec : channel -> string -> unit = "caml_ssh_request_exec"

external send_eof : channel -> unit = "caml_ssh_send_eof"

external set_host : session -> string -> unit = "caml_ssh_set_host"

external set_port : session -> int -> unit = "caml_ssh_set_port"

external set_timeout : session -> int -> unit = "caml_ssh_set_timeout"

external set_user : session -> string -> unit = "caml_ssh_set_user"

let exec channel command =
  request_exec channel command ;
  read channel

let execs session command =
  let channel = create_channel session in
  let response = exec channel command in
  send_eof channel ; close_channel channel ; response
