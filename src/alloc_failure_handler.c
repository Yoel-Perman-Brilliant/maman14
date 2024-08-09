
static unsigned allocation_failure : 1;

void set_alloc_failure() {
    allocation_failure = 1;
}

unsigned is_alloc_failure() {
    return allocation_failure;
}