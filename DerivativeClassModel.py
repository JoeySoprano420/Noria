class Directive:
    def __init__(self, name, args, context):
        self.name = name
        self.args = args
        self.context = context

    def evaluate(self):
        raise NotImplementedError

class PingDirective(Directive):
    def evaluate(self):
        target = self.args[0]
        print(f"[PING] {target}")
        return f"Triggered: {target}"

class EncryptDirective(Directive):
    def evaluate(self):
        plaintext = self.args[0]
        encrypted = AES256.encrypt(plaintext, key="NOIRIA_KEY")
        context.store("cipheredText", encrypted)
        return encrypted
