#ifndef OVAT_IF_H
#define OVAT_IF_H

#define OVAT_IF_ACTION_OK "SUCCESS"
#define OVAT_IF_ACTION_NOT_OK "FAIL"

void ovat_if_init(void *aux);
void ovat_if_uninit(void);
int ovat_if_module_unload(const char *name);
int ovat_if_module_load(const char *name, unsigned int msecs);
void ovat_if_action_reply(int fd, void *aux, const char *action, const char *ret);

#endif /* OVAT_IF_H */
