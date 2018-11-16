type channel

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

val close_channel : channel -> unit
(** Close a channel.
    @param channel The channel to close
    @since 0.1
*)

val create_channel : session -> channel
(** Create a new SSH channel. Can be used to execute remote commands, open
    shells, transfer files, forward ports, etc.
    @param session The session to use
    @return the newly allocated channel
    @since 0.1
*)

val create_session : unit -> session
(** Create a new SSH session.
    @return A new session
    @since 0.1
*)

val exec : channel -> string -> string
(** Execute a command and get the response.
    @param channel A channel to execute the command on. Must be readable and writable.
    @param command The command to execute
    @return The result of the command
    @since 0.1
*)

val execs : session -> string -> string
(** Execute a command and get the response.
    @param session A session to create a channel for
    @param command The command to execute
    @return The result of the command
    @since 0.1
*)

val disconnect : session -> unit
(** Disconnect from a session. The session can then be reused.
    @param session The session to use
    @since 0.1
*)

val read : channel -> string
(** Read data from a channel.
    @param channel The channel to read
    @return The data read
    @since 0.1
*)

val request_exec : channel -> string -> unit
(** Run a command without an interactive shell.
    @param channel The channel to execute the command
    @param command The command to run
    @since 0.1
*)

val send_eof : channel -> unit
(** Send an end of file on the channel. Does not close the channel. You may read
    from it but not write.
    @param channel The channel to send EOF to
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
