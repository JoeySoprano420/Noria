def compile_assign(node):
    # emit: int x = 42
    builder.alloca(ir.IntType(32), name=node.name)
    builder.store(ir.Constant(ir.IntType(32), node.value), node.name)
