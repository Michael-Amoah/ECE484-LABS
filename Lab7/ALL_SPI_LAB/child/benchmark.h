
extern volatile long int overflows;

void start_timer();
double end_timer();
int errors_in_packet(char *received_packet, int size);
