#include <errno.h>
#include <string.h>

#include <caml/alloc.h>
#include <caml/custom.h>
#include <caml/fail.h>
#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <libssh/libssh.h>

static void finalize_session_handle(value session_handle) {
  ssh_session *session = (ssh_session *) Data_custom_val(session_handle);
  ssh_free(*session);
}

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

CAMLprim value caml_ssh_create(void) {
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
