CC:=gcc
create_test:create_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

terminate:terminate.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

pthread_detach_test:pthread_detach_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

pthread_cancel_deferred_test:pthread_cancel_deferred_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

pthread_cancel_async_test:pthread_cancel_async_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

pthread_cancel_disable_test :pthread_cancel_disable_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

race_condition :race_condition.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

rwlock_test_writer_unlock :rwlock_test_writer_unlock.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

rwlock_test :rwlock_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

rwlock_rw_alternate :rwlock_rw_alternate.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

rwlock_write_hungry :rwlock_write_hungry.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

rwlock_hungry_solved :rwlock_hungry_solved.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

condition_var :condition_var.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

unnameed_sem_bin_thread_condition_va :unnameed_sem_bin_thread_condition.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

unnameed_sem_bin_thread :unnameed_sem_bin_thread.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

unnamed_sem_bin_process_condition :unnamed_sem_bin_process_condition.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

unnamed_sem_count_thread :unnamed_sem_count_thread.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

unnamed_sem_count_process :unnamed_sem_count_process.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

named_sem_bin :named_sem_bin.c
	-$(CC) -o $@ $^ -lpthread
	-./$@
	-rm ./$@

named_sem_count :named_sem_count.c
	-$(CC) -o $@ $^ -lpthread
	-./$@
	-rm ./$@