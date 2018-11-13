type session

external auth_pass : session -> string -> bool = "caml_ssh_auth_pass"

external connect : session -> bool = "caml_ssh_connect"

external create : unit -> session = "caml_ssh_create"

external disconnect : session -> unit = "caml_ssh_disconnect"

external set_host : session -> string -> unit = "caml_ssh_set_host"

external set_port : session -> int -> unit = "caml_ssh_set_port"

external set_timeout : session -> int -> unit = "caml_ssh_set_timeout"

external set_user : session -> string -> unit = "caml_ssh_set_user"
