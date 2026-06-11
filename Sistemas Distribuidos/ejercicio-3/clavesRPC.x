
struct PaqueteRPC {
   int x;
   int y;
   int z;
};

struct set_value_arg {
  char key[256];
  char value1[256];
  int N_value2;
  float V_value2[32];
  struct PaqueteRPC value3;
};

struct get_value_ret {
  int status;
  char value1[256];
  int N_value2;
  float V_value2[32];
  struct PaqueteRPC value3;
};

struct key_arg {
  char key[256];
};

program CLAVES {
	version CLAVES_VERSION {
		int rpc_destroy(void) = 1;
		int rpc_set_value(struct set_value_arg) = 2;
		struct get_value_ret rpc_get_value(struct key_arg) = 3;
		int rpc_modify_value(struct set_value_arg) = 4;
		int rpc_delete_key(struct key_arg) = 5;
		int rpc_exist(struct key_arg) = 6;
	} = 1;
} = 100522255;