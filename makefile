COMMIT_MSG ?= Manual-commit: $(shell date +'%Y-%m-%d %H:%M:%S')

help:
	@echo "run 'make push' to push all changes to github"

push:
	@git add .
	@git commit -m "$(COMMIT_MSG)"
	@git push -u origin Eric-branch

p: push
