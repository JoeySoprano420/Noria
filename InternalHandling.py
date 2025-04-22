class ConcurrentEngine:
    def __init__(self):
        self.tasks = []

    def spawn(self, directive):
        self.tasks.append(Thread(target=directive.evaluate))

    def join_all(self):
        for task in self.tasks:
            task.join()
