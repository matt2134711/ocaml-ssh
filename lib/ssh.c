#include <errno.h>
#include <string.h>

#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/fail.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <libssh/libssh.h>

static void finalize_channel_handle(value channel_handle) {
  ssh_channel *channel = (ssh_channel *) Data_custom_val(channel_handle);
  ssh_channel_free(*channel);
}

static void finalize_session_handle(value session_handle) {
  ssh_session *session = (ssh_session *) Data_custom_val(session_handle);
  ssh_free(*session);
}

static struct custom_operations ssh_channel_custom_ops = {
  .identifier = "ssh_channel handling",
  .finalize = finalize_channel_handle,
  .compare = custom_compare_default,
  .hash = custom_hash_default,
  .serialize = custom_serialize_default,
  .deserialize = custom_deserialize_default
};

static struct custom_operations ssh_session_custom_ops = {
  .identifier = "ssh_session handling",
  .finalize = finalize_session_handle,
  .compare = custom_compare_default,
  .hash = custom_hash_default,
  .serialize = custom_serialize_default,
  .deserialize = custom_deserialize_default
};

CAMLprim value caml_ssh_auth_pass(value session_handle, value password_handle) {
  CAMLparam2(session_handle, password_handle);

  ssh_session *session = (ssh_session *) Data_custom_val(session_handle);
  char *password = caml_strdup(String_val(password_handle));

  int rc = ssh_userauth_password(*session, NULL, password);

  caml_stat_free(password);

  CAMLreturn(rc == SSH_AUTH_SUCCESS ? Val_true : Val_false);
}

CAMLprim value caml_ssh_connect(value session_handle) {
  CAMLparam1(session_handle);

  ssh_session *session = (ssh_session *) Data_custom_val(session_handle);

  int rc = ssh_connect(*session);

  CAMLreturn(rc == SSH_OK ? Val_true : Val_false);
}

CAMLprim value caml_ssh_close_channel(value channel_handle) {
  CAMLparam1(channel_handle);

  ssh_channel *channel = (ssh_channel *) Data_custom_val(channel_handle);

  ssh_channel_close(*channel);

  CAMLreturn(Val_unit);
}

CAMLprim value caml_ssh_create_channel(value session_handle) {
  CAMLparam1(session_handle);
  CAMLlocal1(channel_handle);

  ssh_session *session = (ssh_session *) Data_custom_val(session_handle);

  ssh_channel channel = ssh_channel_new(*session);
  if (!channel) {
    caml_failwith("Failed to allocate channel");
  }

  // TODO: extract to new method
  if (ssh_channel_open_session(channel) != SSH_OK) {
    caml_failwith("Failed to open a session channel");
  }

  channel_handle = caml_alloc_custom(&ssh_channel_custom_ops, sizeof(&channel), 0, 1);
  memcpy(Data_custom_val(channel_handle), &channel, sizeof(&channel));

  CAMLreturn(channel_handle);
}

CAMLprim value caml_ssh_create_session(void) {
  CAMLparam0();
  CAMLlocal1(session_handle);

  ssh_session session = ssh_new();
  if (!session) {
    caml_failwith(strerror(errno));
  }

  session_handle = caml_alloc_custom(&ssh_session_custom_ops, sizeof(&session), 0, 1);
  memcpy(Data_custom_val(session_handle), &session, sizeof(&session));

  CAMLreturn(session_handle);
}

CAMLprim value caml_ssh_disconnect(value session_handle) {
  CAMLparam1(session_handle);

  ssh_session *session = (ssh_session *) Data_custom_val(session_handle);

  ssh_disconnect(*session);

  CAMLreturn(Val_unit);
}

CAMLprim value caml_ssh_read(value channel_handle) {
  CAMLparam1(channel_handle);
  CAMLlocal1(response_handle);

  ssh_channel *channel = (ssh_channel *) Data_custom_val(channel_handle);

  char current_read[512];
  int current_read_bytes = 0;
  char *buf = malloc(512);
  int buf_size = 512;
  int total_bytes_read = 0;

  if (!buf) {
    caml_failwith("Failed to allocate memory");
  }

  while ((current_read_bytes = ssh_channel_read(*channel, current_read, 512, 0)) > 0) {
    if (total_bytes_read + current_read_bytes > buf_size) {
      buf_size *= 2;
      buf = realloc(buf, buf_size);
      if (!buf) {
        caml_failwith("Failed to allocate memory");
      }
    }
    strncpy((buf + total_bytes_read), current_read, current_read_bytes);
    total_bytes_read += current_read_bytes;
  }

  if (total_bytes_read + 1 > buf_size) {
    buf_size++;
    buf = realloc(buf, buf_size);
    if (!buf) {
      caml_failwith("Failed to allocate memory");
    }
  }

  buf[total_bytes_read] = '\0';

  response_handle = caml_copy_string(buf);

  free(buf);

  CAMLreturn(response_handle);
}

CAMLprim value caml_ssh_request_exec(value channel_handle, value command_handle) {
  CAMLparam2(channel_handle, command_handle);

  ssh_channel *channel = (ssh_channel *) Data_custom_val(channel_handle);
  char *command = caml_strdup(String_val(command_handle));

  if (ssh_channel_request_exec(*channel, command) != SSH_OK) {
    caml_failwith("Failed to exec");
  }

  caml_stat_free(command);

  CAMLreturn(Val_unit);
}

CAMLprim value caml_ssh_send_eof(value channel_handle) {
  CAMLparam1(channel_handle);

  ssh_channel *channel = (ssh_channel *) Data_custom_val(channel_handle);

  ssh_channel_send_eof(*channel);

  CAMLreturn(Val_unit);
}

CAMLprim value caml_ssh_set_host(value session_handle, value host_handle) {
  CAMLparam2(session_handle, host_handle);

  ssh_session *session = (ssh_session *) Data_custom_val(session_handle);
  char *host = caml_strdup(String_val(host_handle));

  ssh_options_set(*session, SSH_OPTIONS_HOST, host);

  caml_stat_free(host);

  CAMLreturn(Val_unit);
}

CAMLprim value caml_ssh_set_port(value session_handle, value port_handle) {
  CAMLparam2(session_handle, port_handle);

  ssh_session *session = (ssh_session *) Data_custom_val(session_handle);
  int port = Int_val(port_handle);

  ssh_options_set(*session, SSH_OPTIONS_PORT, &port);

  CAMLreturn(Val_unit);
}

CAMLprim value caml_ssh_set_timeout(value session_handle, value timeout_handle) {
  CAMLparam2(session_handle, timeout_handle);

  ssh_session *session = (ssh_session *) Data_custom_val(session_handle);
  int timeout = Int_val(timeout_handle);

  ssh_options_set(*session, SSH_OPTIONS_TIMEOUT_USEC, &timeout);

  CAMLreturn(Val_unit);
}

CAMLprim value caml_ssh_set_user(value session_handle, value user_handle) {
  CAMLparam2(session_handle, user_handle);

  ssh_session *session = (ssh_session *) Data_custom_val(session_handle);
  char *user = caml_strdup(String_val(user_handle));

  ssh_options_set(*session, SSH_OPTIONS_USER, user);

  caml_stat_free(user);

  CAMLreturn(Val_unit);
}
