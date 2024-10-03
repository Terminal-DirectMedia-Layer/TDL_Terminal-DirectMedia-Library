
# Contributing to TDL

## Introduction

Join us in the development of TDL! We are always looking for new contributors to help us improve the library. And there is many way to contribute even if you are new to the library or even new to programming.

This might be scary to make the first step. But don't worry, an active community is here to help you.

So, let's find which contribution option fits you the best and help you join the development of TDL!

## Ways to contribute

- **Spread the word**: You can help us by talking about TDL to your friends, on social media or even on your blog. This will help us to make the library more popular and attract new contributors.
- **Star the project**: You can help us by starring the project on [GitHub](https://github.com/Terminal-DirectMedia-Layer/TDL_Terminal-DirectMedia-Library) to make it more popular and more visible.
- **Report bugs**: You can help us by reporting bugs on the [GitHub issue tracker](https://github.com/Terminal-DirectMedia-Layer/TDL_Terminal-DirectMedia-Library/issues)
- **Tell us your ideas**: You can help us by suggesting new features on the [GitHub issue tracker](https://github.com/Terminal-DirectMedia-Layer/TDL_Terminal-DirectMedia-Library/issues)
- **Contribute to the code**: You can help us by developing new features. See below for more details.

## Pull Request

Merging your fix to the TDL project or any tier project like the documentation is done by a Pull Request.
It's a method to propose your changes to the project. To do that, you have to make a copy of the project (called a fork) on your gitHub account.

To add your changes you can edit files online on GitHub and send a new Pull request from there (recommended for small changes) or add the updates in your favorite editor/IDE and use git to publish the changes (recommended for more complex updates).

### Fork from your computer

The instructions describe the main tdl repository `but it works the same way for the other repositories.

1. Fork the tdl repository. To do this click the "Fork" button in the top right corner. It will "copy" the tdl repository to your GitHub account (https://github.com/`YOUR_NAME`/tdl).
2. Clone your forked repository.
3. Add your changes. You can create a feature branch from master for the updates: git checkout -b the-new-feature-branch-name
4. Commit and push your changes to the forked tdl repository.
5. Create a PR on GitHub from the page of your tdl repository (`https://github.com/`YOUR_NAME`/tdl`) by clicking the "New pull request" button. Don't forget to select the branch where you added your changes.
6. Set the base branch. It means where you want to merge your update. In the tdl repo both the fixes and new features go to master branch.
7. Describe what is in the update. An example code is welcome if applicable.
8. If you need to make more changes, just update your forked tdl repo with new commits. They will automatically appear in the PR.

### Commit message format

The commit messages format is inspired from [Angular commit message convention]( https://gist.github.com/brianclements/841ea7bffdb01346392c)

When commiting changes, the following structure should be used in the title and associated description of said commit:

```
<type>(<scope>): <subject>
<BLANK LINE>
<body>
<BLANK LINE>
<footer>
```

The possible commit **`<type>`**s are:
- **`feat`**: new feature
- **`core`**: architectural changes
- **`perf`**: changes that affect the performance
- **`example`**: anything related to examples (even fixes and new examples)
- **`docs`**: anything related to the documentation (even fixes, formatting, and new pages)
- **`test`**: anything related to tests (new and updated tests or CI actions)
- **`refactor`**: A code change that neither fixes a bug nor adds a feature
- **`<scope>`**: is optional and can be anything specifying the place of the change. For example, *sound* , *input*, *display*. This scope can be ommited.
- **`<subject>`**: is a short description of the change :
  - use the imperative, present tense: "change" not "changed" nor "changes"
  - don't capitalize the first letter
  - no dot (.) at the end
  - 80 characters max
- **`<body>`**: optional and can be used to describe the details of this change.
- **`<footer>`**: shall contain the issue number and/or link if applicable.