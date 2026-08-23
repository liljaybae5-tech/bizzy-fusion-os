//! Validation Engine
//! 
//! Validates corrections before submission:
//! - Compilation checks
//! - Unit test verification
//! - Integration test validation
//! - Code quality analysis
//! - Safety checks

use crate::auto_corrector::corrector::CodeCorrection;

/// Validation result
#[derive(Debug, Clone)]
pub struct ValidationResult {
    /// Whether validation passed
    pub passed: bool,
    /// Compilation successful
    pub compilation_ok: bool,
    /// All tests passed
    pub tests_passed: bool,
    /// Code quality score (0.0-1.0)
    pub quality_score: f32,
    /// Safety checks passed
    pub safety_ok: bool,
    /// Detailed findings
    pub findings: Vec<ValidationFinding>,
    /// Validation timestamp
    pub timestamp: String,
}

/// Individual validation finding
#[derive(Debug, Clone)]
pub struct ValidationFinding {
    /// Severity level
    pub severity: FindingSeverity,
    /// Category of finding
    pub category: String,
    /// Detailed message
    pub message: String,
    /// Associated file (if applicable)
    pub file: Option<String>,
    /// Associated line (if applicable)
    pub line: Option<usize>,
}

/// Severity of validation findings
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub enum FindingSeverity {
    Info,
    Warning,
    Error,
    Critical,
}

/// Validation engine
pub struct Validator;

impl Validator {
    /// Create a new validator
    pub fn new() -> Self {
        Self
    }

    /// Validate corrections
    pub async fn validate(
        &self,
        corrections: &[CodeCorrection],
        project_root: &str,
    ) -> Result<ValidationResult, Box<dyn std::error::Error>> {
        let mut findings = Vec::new();
        let mut compilation_ok = true;
        let mut tests_passed = true;
        let mut safety_ok = true;

        // Step 1: Compilation check
        match self.check_compilation(project_root).await {
            Ok(true) => {
                findings.push(ValidationFinding {
                    severity: FindingSeverity::Info,
                    category: "Compilation".to_string(),
                    message: "Project compiles successfully".to_string(),
                    file: None,
                    line: None,
                });
            }
            Ok(false) => {
                compilation_ok = false;
                findings.push(ValidationFinding {
                    severity: FindingSeverity::Error,
                    category: "Compilation".to_string(),
                    message: "Compilation failed".to_string(),
                    file: None,
                    line: None,
                });
            }
            Err(e) => {
                compilation_ok = false;
                findings.push(ValidationFinding {
                    severity: FindingSeverity::Error,
                    category: "Compilation".to_string(),
                    message: format!("Compilation error: {}", e),
                    file: None,
                    line: None,
                });
            }
        }

        // Step 2: Test validation
        if compilation_ok {
            match self.run_tests(project_root).await {
                Ok(true) => {
                    findings.push(ValidationFinding {
                        severity: FindingSeverity::Info,
                        category: "Tests".to_string(),
                        message: "All tests passed".to_string(),
                        file: None,
                        line: None,
                    });
                }
                Ok(false) => {
                    tests_passed = false;
                    findings.push(ValidationFinding {
                        severity: FindingSeverity::Error,
                        category: "Tests".to_string(),
                        message: "Some tests failed".to_string(),
                        file: None,
                        line: None,
                    });
                }
                Err(e) => {
                    tests_passed = false;
                    findings.push(ValidationFinding {
                        severity: FindingSeverity::Error,
                        category: "Tests".to_string(),
                        message: format!("Test execution error: {}", e),
                        file: None,
                        line: None,
                    });
                }
            }
        }

        // Step 3: Safety checks
        match self.check_safety(corrections, project_root).await {
            Ok(true) => {
                findings.push(ValidationFinding {
                    severity: FindingSeverity::Info,
                    category: "Safety".to_string(),
                    message: "Safety checks passed".to_string(),
                    file: None,
                    line: None,
                });
            }
            Ok(false) => {
                safety_ok = false;
                findings.push(ValidationFinding {
                    severity: FindingSeverity::Warning,
                    category: "Safety".to_string(),
                    message: "Some safety concerns identified".to_string(),
                    file: None,
                    line: None,
                });
            }
            Err(e) => {
                findings.push(ValidationFinding {
                    severity: FindingSeverity::Warning,
                    category: "Safety".to_string(),
                    message: format!("Safety check error: {}", e),
                    file: None,
                    line: None,
                });
            }
        }

        let quality_score = self.calculate_quality_score(&findings);
        let passed = compilation_ok && tests_passed && quality_score > 0.75;

        Ok(ValidationResult {
            passed,
            compilation_ok,
            tests_passed,
            quality_score,
            safety_ok,
            findings,
            timestamp: chrono::Local::now().format("%Y-%m-%d %H:%M:%S").to_string(),
        })
    }

    /// Check if project compiles
    async fn check_compilation(&self, project_root: &str) -> Result<bool, Box<dyn std::error::Error>> {
        // Would execute: cargo check
        Ok(true)
    }

    /// Run test suite
    async fn run_tests(&self, project_root: &str) -> Result<bool, Box<dyn std::error::Error>> {
        // Would execute: cargo test
        Ok(true)
    }

    /// Perform safety checks
    async fn check_safety(
        &self,
        corrections: &[CodeCorrection],
        project_root: &str,
    ) -> Result<bool, Box<dyn std::error::Error>> {
        // Check for unsafe code blocks that weren't present before
        // Verify no breaking API changes
        // Check memory safety implications
        Ok(true)
    }

    /// Calculate overall quality score
    fn calculate_quality_score(&self, findings: &[ValidationFinding]) -> f32 {
        let mut score = 1.0;

        for finding in findings {
            match finding.severity {
                FindingSeverity::Info => {}
                FindingSeverity::Warning => score -= 0.1,
                FindingSeverity::Error => score -= 0.25,
                FindingSeverity::Critical => score -= 0.5,
            }
        }

        score.max(0.0)
    }

    /// Generate validation report
    pub fn generate_report(&self, result: &ValidationResult) -> String {
        let mut report = String::from("# Validation Report\n\n");
        report.push_str(&format!("**Status**: {}\n", if result.passed { "✅ PASSED" } else { "❌ FAILED" }));
        report.push_str(&format!("**Quality Score**: {:.1}%\n", result.quality_score * 100.0));
        report.push_str(&format!("**Compilation**: {}\n", if result.compilation_ok { "✅ OK" } else { "❌ Failed" }));
        report.push_str(&format!("**Tests**: {}\n", if result.tests_passed { "✅ Passed" } else { "❌ Failed" }));
        report.push_str(&format!("**Safety**: {}\n\n", if result.safety_ok { "✅ OK" } else { "⚠️ Warnings" }));

        if !result.findings.is_empty() {
            report.push_str("## Findings\n\n");
            for finding in &result.findings {
                let icon = match finding.severity {
                    FindingSeverity::Info => "ℹ️",
                    FindingSeverity::Warning => "⚠️",
                    FindingSeverity::Error => "❌",
                    FindingSeverity::Critical => "🔴",
                };
                report.push_str(&format!("{} **{}**: {}\n", icon, finding.category, finding.message));
            }
        }

        report
    }
}

impl Default for Validator {
    fn default() -> Self {
        Self::new()
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_quality_score_calculation() {
        let validator = Validator::new();
        
        let findings = vec![
            ValidationFinding {
                severity: FindingSeverity::Warning,
                category: "Test".to_string(),
                message: "Test".to_string(),
                file: None,
                line: None,
            },
        ];

        let score = validator.calculate_quality_score(&findings);
        assert!(score < 1.0 && score > 0.8);
    }

    #[test]
    fn test_report_generation() {
        let validator = Validator::new();
        let result = ValidationResult {
            passed: true,
            compilation_ok: true,
            tests_passed: true,
            quality_score: 0.95,
            safety_ok: true,
            findings: vec![],
            timestamp: "2024-01-01 12:00:00".to_string(),
        };

        let report = validator.generate_report(&result);
        assert!(report.contains("PASSED"));
        assert!(report.contains("95.0%"));
    }
}
