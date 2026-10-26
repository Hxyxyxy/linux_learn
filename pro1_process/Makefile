CC := gcc

fork_test:fork_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

fork_fd_test:fork_fd_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

erlou:erlou.c
	-$(CC) -o $@ $^

execve_test:execve_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

fork_execve_test:fork_execve_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

waitpid_test:waitpid_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

pstree_test:pstree_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

erlou_block: erlou_block.c
	-$(CC) -o $@ $^

orphan_process_test:orphan_process_test.c
	-$(CC) -o $@ $^
	-./$@
	-rm ./$@

unnameed_pipe_test:unnameed_pipe_test.c
	-$(CC) -o $@ $^
	-./$@ "test"
	-rm ./$@

fifo_write:fifo_write.c
	-$(CC) -o $@ $^

fifo_read:fifo_read.c
	-$(CC) -o $@ $^

shard_memory:shard_memory.c
	-$(CC) -o $@ $^
	-./$@ "test"
	-rm ./$@
