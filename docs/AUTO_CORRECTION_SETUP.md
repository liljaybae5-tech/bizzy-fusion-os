# Auto-Correction Agent Setup Guide

## Prerequisites

- Rust 1.70+ toolchain
- GitHub Actions enabled in repository
- GitHub token with `contents:write` and `pull_requests:write` permissions
- Project must compile with Rust

## Installation Steps

### 1. Update Cargo.toml

Add required dependencies:

```toml
[dependencies]
tokio = { version = "1.0", features = ["full"] }
chrono = "0.4"
serde = { version = "1.0", features = ["derive"] }
serde_json = "1.0"
octocat = "0.1"  # GitHub API client

[dev-dependencies]
# Testing dependencies
```

### 2. Initialize Module

Add to `src/lib.rs`:

```rust
pub mod auto_corrector;
```

Or in `src/main.rs` if building as binary:

```rust
mod auto_corrector;

#[tokio::main]
async fn main() -> Result<(), Box<dyn std::error::Error>> {
    let config = auto_corrector::AutoCorrectorConfig::default();
    let agent = auto_corrector::AutoCorrectionAgent::new(config);
    agent.initialize().await?;
    agent.run().await?;
    Ok(())
}
```

### 3. Deploy Workflow

1. Copy `.github/workflows/auto-correction.yml` to your repository
2. Ensure file location is exactly: `.github/workflows/auto-correction.yml`
3. Commit and push to repository

### 4. Configure Permissions

In GitHub repository settings:

**Settings → Workflow permissions:**
- ✅ Read and write permissions
- ✅ Allow GitHub Actions to create and approve pull requests

### 5. Set GitHub Token (if using separate account)

**Settings → Secrets and variables → Actions:**

Add or verify `GITHUB_TOKEN` is available (GitHub provides this automatically).

Optionally add personal token:
```
GITHUB_TOKEN = your_personal_access_token
```

## Configuration

### Using Default Configuration

The agent works with sensible defaults:

```rust
let config = AutoCorrectorConfig::default();
// auto_pr_enabled: true
// min_confidence: 0.85
// target_branch: "main"
// validate_before_pr: true
```

### Custom Configuration

```rust
let config = AutoCorrectorConfig {
    auto_pr_enabled: true,
    min_confidence: 0.90,  // Higher threshold
    target_branch: "develop".to_string(),
    validate_before_pr: true,
};

let agent = AutoCorrectionAgent::new(config);
```

### Via Configuration File

Create `.github/auto-corrector.yml`:

```yaml
min_confidence: 0.85
auto_pr_enabled: true
target_branch: main
validate_before_pr: true
```

## First Run

### Manual Trigger

1. Go to **Actions** tab in GitHub
2. Select **Auto-Correction Agent** workflow
3. Click **Run workflow**
4. Select desired severity level
5. Click **Run workflow**

The workflow will:
1. Scan project for issues
2. Apply corrections
3. Validate changes
4. Create a pull request (if issues found and passing validation)

### Monitor Execution

- Check workflow logs in Actions tab
- Watch for created pull requests
- Review PR details and validation results
- Merge after review or close if needed

## Customization

### Adjust Confidence Threshold

Edit `.github/auto-corrector.yml`:

```yaml
min_confidence: 0.90  # More conservative
# or
min_confidence: 0.75  # More aggressive
```

### Add Reviewers

In workflow file, update job:

```yaml
add-comment-with-validation-results:
  # ...
  steps:
    - name: Request reviewers
      uses: actions/github-script@v7
      with:
        script: |
          github.rest.pulls.requestReviewers({
            owner: context.repo.owner,
            repo: context.repo.repo,
            pull_number: pr.number,
            reviewers: ['username1', 'username2']
          });
```

### Customize Labels

Edit workflow's `create-pull-request` job:

```yaml
labels: ['automated', 'auto-fix', 'code-quality', 'custom-label']
```

### Disable for Specific Files

Add to corrector config:

```rust
let excluded_paths = vec![
    "tests/",
    "examples/",
    "build/",
];
```

## Verification

### Check Installation

```bash
# Build project
cargo build

# Run tests
cargo test auto_corrector

# Check workflow syntax
gh workflow view auto-correction
```

### Test Configuration

Create test scenario:

```bash
# Introduce intentional error
echo "let x = y" > test_error.rs  # Missing semicolon

# Trigger workflow
gh workflow run auto-correction.yml

# Check results
gh pr list --state open
```

## Troubleshooting

### Workflow Not Running

**Problem:** Workflow doesn't appear in Actions tab

**Solution:**
- Verify `.github/workflows/auto-correction.yml` exists
- Check branch is main/master
- Ensure YAML syntax is valid
- Trigger manually: `gh workflow run auto-correction.yml`

### No PR Created Despite Issues

**Problem:** Workflow runs but no PR appears

**Solution:**
- Check workflow logs for validation failures
- Verify `auto_pr_enabled: true`
- Ensure GitHub token has PR creation permissions
- Check if issues have confidence >= threshold

### Permission Denied

**Problem:** "Resource not accessible by integration"

**Solution:**
- **Settings → Workflow permissions:**
  - ✅ Set to "Read and write permissions"
  - ✅ Check "Allow GitHub Actions to create pull requests"
- Use personal token instead of automatic GITHUB_TOKEN if needed

### Tests Failing After Corrections

**Problem:** Auto-corrections break tests

**Solution:**
- Increase `min_confidence` threshold
- Review failed PR and provide feedback
- Add custom validation rules
- Adjust correction strategies

## Monitoring and Maintenance

### Regular Checks

- **Weekly:** Review auto-correction PRs
- **Monthly:** Check confidence scores and accuracy
- **Quarterly:** Evaluate effectiveness and ROI

### Performance Optimization

- Enable caching in workflow
- Exclude unchanged files
- Batch process corrections
- Schedule during off-peak hours

### Metrics Dashboard

Track:
- Issues detected per run
- Correction success rate (0-100%)
- Quality score trends
- Time to create PR
- PR merge rate

## Next Steps

1. **Schedule Workflow:** Set cron schedule in workflow file
2. **Set Up Notifications:** Configure Slack/Email alerts
3. **Integrate with CI:** Add checks to PR workflow
4. **Team Training:** Educate team on auto-correction process
5. **Iterate:** Adjust configuration based on results

## Support

For issues or questions:
1. Check workflow logs (Actions tab)
2. Review `AUTO_CORRECTION_AGENT.md` documentation
3. Check GitHub Actions documentation
4. File issue in repository with workflow logs
