void scheduler(void) {
    struct proc *p;
    struct cpu *c = mycpu();
    c->proc = 0;

    for (;;) {
        // Enable interrupts on this processor.
        sti();

        // Acquire the process table lock.
        acquire(&ptable.lock);

        // Check all processes to find one to run.
        for (p = ptable.proc; p < &ptable.proc[NPROC]; p++) {
            if (p->state != RUNNABLE) {
                continue;
            }

            // Find the next process to run from the RBT
            struct proc* nextProc = getProcess(&rbtRoot);
            if (nextProc == NULL) {
                continue; // No process found, continue checking
            }

            // Switch to chosen process
            c->proc = nextProc;
            switchuvm(nextProc);
            nextProc->state = RUNNING;

            swtch(&(c->scheduler), nextProc->context);
            switchkvm();

            // Process is done running
            c->proc = 0;

            // Update virtual runtime and reinsert into RBT
            updateVirtualRuntime(nextProc);
            insertProcess(&rbtRoot, nextProc);
        }

        // Release the process table lock.
        release(&ptable.lock);
    }
}

void updateVirtualRuntime(struct proc* p) {
    // Update the virtual runtime of the process
    // This typically involves adding the time spent running
    // adjusted by the process's weight
    p->vruntime += ... // Compute based on the time quantum and weight
}