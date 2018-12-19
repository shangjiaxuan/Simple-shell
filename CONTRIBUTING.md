# Contributing

Looking to contribute something? **Here's how you can help.**

Please take a moment to review this document in order to make the contribution
process easy and effective for everyone involved.

Following these guidelines helps to communicate that you respect the time of
the developers managing and developing this open source project. In return,
they should reciprocate that respect in addressing your issue or assessing
patches and features.

NOTE:  
You can always make your own extensions as exemplified in the `sjx.dll`. You can use a 
normal interface like a "int main(int argc, char** argv)" or you can directly add your 
extension to the code to use any set of input and output for your function. In the latter 
case, you will need to be responsible for the maintenence and safery of your extension.

## Using the issue tracker

The issue tracker is the preferred channel for [bug reports](#bug-reports),
[features requests](#feature-requests) and
[submitting pull requests](#pull-requests), but please respect the
following restrictions:

* Please **do not** derail or troll issues. Keep the discussion on topic and
  respect the opinions of others.

* Please **do not** open issues or pull requests which *belongs to* third party
  components.  
  (However, integration of your code as extension here directly will make you a contributer, 
  and thus making you responsible for maintaining your extension)


## Bug reports

A bug is a _demonstrable problem_ that is caused by the code in the repository.
Good bug reports are extremely helpful, so thanks!

Guidelines for bug reports:

1. **Use the GitHub issue search** &mdash; check if the issue has already been
   reported.

2. **Check if the issue has been fixed** &mdash; try to reproduce it using the
   latest `master` or development branch in the repository.

3. **Isolate the problem** &mdash; ideally create an
   [SSCCE](http://www.sscce.org/) and a live example.
   Uploading the project on cloud storage (OneDrive, DropBox, et el.)
   or creating a sample GitHub repository is also helpful.


A good bug report shouldn't leave others needing to chase you up for more
information. Please try to be as detailed as possible in your report. What is
your environment? What steps will reproduce the issue? What browser(s) and OS
experience the problem? Do other browsers show the bug differently? What
would you expect to be the outcome? All these details will help people to fix
any potential bugs.

Example:

> Short and descriptive example bug report title
>
> A summary of the issue and the Visual Studio, browser, OS environments
> in which it occurs. If suitable, include the steps required to reproduce the bug.
>
> 1. This is the first step
> 2. This is the second step
> 3. Further steps, etc.
>
> `<url>` - a link to the project/file uploaded on cloud storage or other publicly accessible medium.
>
> Any other information you want to share that is relevant to the issue being
> reported. This might include the lines of code that you have identified as
> causing the bug, and potential solutions (and your opinions on their
> merits).


## Feature requests

Feature requests are welcome. Please provide as much detail and context as possible.  
Feature requests for integrating your own extension will always be direct and easy, 
if you take care of your part of wrapping the extension into a dll, and even more so if your
interface is a "int main(int argc, char** argv)", which will only need a addition to the documentation.
Feature requests for new commands will be considered if the request is valued important or if the
you would provide simple code guidelines for integrating them.


## Pull requests

Good pull requests, patches, improvements and new features are a fantastic
help. Any help on adding features and resolving bugs is welcome as long as 
you push it to the develop branch.

**Please ask first** before embarking on any significant pull request (e.g.
implementing features, refactoring code, porting to a different language),
otherwise you risk spending a lot of time working on something that the
project's developers might not want to merge into the project. (Although it
is quite unlikely right now.)

Please adhere to the [coding guidelines](#code-guidelines) used throughout the
project (format, good practices, etc.) and any other requirements.

Adhering to the following process is the best way to get your work
included in the project:

1. [Fork](http://help.github.com/fork-a-repo/) the project, clone your fork,
   and configure the remotes:

   ```bash
   # Clone your fork of the repo into the current directory
   git clone https://github.com/<your-username>/<this-repro-name>.git
   # Navigate to the newly cloned directory
   cd <folder-name>
   # Assign the original repo to a remote called "upstream"
   git remote add upstream https://github.com/madskristensen/<this-repro-name>.git
   ```

2. If you cloned a while ago, get the latest changes from upstream:

   ```bash
   git checkout develop
   git pull upstream develop
   ```

3. Create a new topic branch (off the main project development branch) to
   contain your feature, change, or fix:

   ```bash
   git checkout -b <topic-branch-name>
   ```

4. Commit your changes in logical chunks. Please adhere to these [git commit
   message guidelines](http://tbaggery.com/2008/04/19/a-note-about-git-commit-messages.html)
   or your code is unlikely be merged into the main project. Use Git's
   [interactive rebase](https://help.github.com/articles/interactive-rebase)
   feature to tidy up your commits before making them public. Also, prepend name of the feature
   to the commit message. For instance: "SCSS: Fixes compiler results for IFileListener.\nFixes `#123`"

5. Locally merge (or rebase) the upstream development branch into your topic branch:

   ```bash
   git pull [--rebase] upstream develop
   ```

6. Push your topic branch up to your fork:

   ```bash
   git push origin <topic-branch-name>
   ```

7. [Open a Pull Request](https://help.github.com/articles/using-pull-requests/)
    with a clear title and description against the `develop` branch.


## Code guidelines

- Indent when needed. Tabs and 4 spaces are both acceptable. 
Please configure the editor to 4 spaces at least for this project for consistency .
- Variable, class, namespace, function, etc's naming has no specific guidelines, but names desperately decorated like `tHeapProACH_1O1`(a.k.a. "the approach 101", in English...) are **NOT accepted** without revision  
Generally, use underscores, upper and lower case switches to convey a sense of seperation of "words", and try to convey a concise and exact idea in variable names if possible. 
- Try to fit the code within at most 150 characters per line. 80 or 120 limit is prefered, but not enforced.
- Modifications to templates are welcome, but should preserve the previous interface or you will need to file an issue before proceeding because dependent code may need a lot of revision.
- Try to follow common cpp guidelines in the mechanics of methods.  
e.g.  
**Try NOT to throw exceptions in destructors**  
**Try NOT to leave memory leaks and random pointers after deleting**  
**Try to be thread safe and exception safe where critical**  

NOTE: Any attempt to add unsafe extensions that may case data loss, break down of the OS, 
and other failures and undesired behaviors without warning will be banned from contributing to this project.

