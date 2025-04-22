class ASTNode:
    def __init__(self, node_type, value=None, children=None):
        self.node_type = node_type
        self.value = value
        self.children = children or []

class AssignNode(ASTNode):
    def __init__(self, var_type, name, expression):
        super().__init__('assign')
        self.var_type = var_type
        self.name = name
        self.expression = expression
