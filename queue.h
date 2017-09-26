

queue_t* sq_create();
int  sq_write(struct message*,queue_t*);
int  sq_read(struct message **,queue_t*);
void sq_delete(queue_t*);

