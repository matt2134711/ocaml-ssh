type session

val auth_pass : session -> string -> bool
(** Try to authenticate by password. Usually disabled on an SSHv2 server.
    @param session The session to use
    @param password The password to authenticate with
    @return true on success, false on failure
    @since 0.1
*)

val connect : session -> bool
(** Connect to the SSH server.
    @param session The session to use
    @return true on success, false on failure
    @since 0.1
*)

val create : unit -> session
(** Create a new ssh session.
    @return A new session
    @since 0.1
*)

val disconnect : session -> unit
(** Disconnect from a session. The session can then be reused.
    @param session The session to use
    @since 0.1
*)

val set_host : session -> string -> unit
(** Set the host to connect to.
    @param session The session to use
    @param hostname The new hostname
    @since 0.1
*)

val set_port : session -> int -> unit
(** Set the port to connect to.
    @param session The session to use
    @param port The new port
    @since 0.1
*)

val set_timeout : session -> int -> unit
(** Set a timeout for the connection in microseconds.
    @param session The session to use
    @param timeout The new timeout, in microseconds
    @since 0.1
*)

val set_user : session -> string -> unit
(** Set the username for authentication.
    @param session The session to use
    @param user The new username
    @since 0.1
*)
