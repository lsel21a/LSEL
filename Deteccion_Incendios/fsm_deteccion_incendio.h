struct  _fsm_valores_t {
    fsm_t fsm;
    int *temperatura;
    int *humedad;
    int *gases;
    };
typedef  struct  _fsm_valores_t  fsm_valores_t ;

void  fsm_valores_init ( fsm_valores_t * f, int *temperatura, int *humedad, int *gases);

# endif  // FSM_REBOUND_H