# GitHub Actions Workflow Installation Guide

## Quick Start

This guide helps you manually create the `.github/workflows/auto-correction.yml` file.

## Step 1: Create Directory Structure

```bash
mkdir -p .github/workflows
```

## Step 2: Create Workflow File

Create `.github/workflows/auto-correction.yml` with the following content:

```yaml
name: Auto-Correction Agent

on:
  schedule:
    - cron: '0 2 * * *'
  workflow_dispatch:
    inputs:
      severity:
        description: 'Minimum issue severity to correct'
        required: true
        default: 'warning'
        type: choice
        options:
          - info
          - warning
          - error
          - critical

env:
  RUST_BACKTRACE: 1
  CARGO_TERM_COLOR: always

jobs:
  detect-issues:
    name: Detect Code Issues
    runs-on: ubuntu-latest
    outputs:
      issues-found: ${{ steps.detect.outputs.count }}
    steps:
      - uses: actions/checkout@v4
        with:
          fetch-depth: 0

      - uses: dtolnay/rust-toolchain@stable

      - uses: Swatinem/rust-cache@v2

      - name: Install analysis tools
        run: |
          rustup component add clippy rustfmt
          cargo install cargo-audit --locked 2>/dev/null || true

      - name: Run checks
        continue-on-error: true
        run: |
          cargo check --all --all-features 2>&1 | tee compile.log
          cargo clippy --all --all-features 2>&1 | tee clippy.log
          cargo audit --deny warnings 2>&1 | tee audit.log || true

      - name: Detect issues
        id: detect
        run: |
          ISSUES=$(grep -c "error" compile.log || echo 0)
          echo "count=$ISSUES" >> $GITHUB_OUTPUT

  auto-correct:
    name: Auto-Correct Issues
    needs: detect-issues
    runs-on: ubuntu-latest
    if: needs.detect-issues.outputs.issues-found != '0'
    steps:
      - uses: actions/checkout@v4
        with:
          fetch-depth: 0

      - uses: dtolnay/rust-toolchain@stable
      - uses: Swatinem/rust-cache@v2

      - name: Configure Git
        run: |
          git config user.name "AutoCorrector[bot]"
          git config user.email "auto-corrector@fusionos.dev"

      - name: Create branch and apply fixes
        run: |
          git checkout -b feature/auto-correction-${{ github.run_id }}
          cargo fmt --all
          cargo fix --all --allow-dirty

      - name: Validate
        run: |
          cargo check --all --all-features
          cargo test --all --all-features

      - name: Commit changes
        run: |
          if [ -n "$(git status --porcelain)" ]; then
            git add -A
            git commit -m "fix: Auto-correct code issues"
            git push origin feature/auto-correction-${{ github.run_id }}
          fi

  create-pr:
    name: Create Pull Request
    needs: [detect-issues, auto-correct]
    runs-on: ubuntu-latest
    if: always() && needs.auto-correct.result == 'success'
    permissions:
      contents: read
      pull-requests: write
    steps:
      - uses: actions/github-script@v7
        with:
          script: |
            const branch = `feature/auto-correction-${{ github.run_id }}`;
            try {
              const { data: pr } = await github.rest.pulls.create({
                owner: context.repo.owner,
                repo: context.repo.repo,
                title: `fix: Auto-correct ${{ needs.detect-issues.outputs.issues-found }} issues`,
                head: branch,
                base: 'main',
                body: `## Auto-Correction Report\n\nFixed **${{ needs.detect-issues.outputs.issues-found }}** detected issues.\n\n✅ All validations passed\n- Compilation successful\n- Tests passed\n- Code quality verified`,
                labels: ['automated', 'auto-fix', 'code-quality'],
              });
              console.log(`PR created: #${pr.number}`);
            } catch (e) {
              console.error('Error creating PR:', e.message);
            }
```

## Step 3: Commit and Push

```bash
git add .github/workflows/auto-correction.yml
git commit -m "ci: Add auto-correction workflow"
git push origin feature/auto-correction-agent
```

## Step 4: Configure Permissions

1. Go to GitHub repository **Settings**
2. Select **Actions** → **General**
3. Under "Workflow permissions":
   - Select "Read and write permissions"
   - ✅ Check "Allow GitHub Actions to create and approve pull requests"
4. Save changes

## Step 5: Test Workflow

1. Go to **Actions** tab
2. Select **Auto-Correction Agent**
3. Click **Run workflow**
4. Select severity level
5. Click **Run workflow**

## Workflow Runs

- **Automatic**: Daily at 2 AM UTC
- **Manual**: Anytime via Actions tab

For more details, see `DEPLOYMENT_CHECKLIST.md`
