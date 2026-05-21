# UnitConverter_15 — Phase 6 BCE/TDD 작업 보고서

| 항목 | 내용 |
|------|------|
| **프로젝트** | UnitConverter_15 |
| **보고서 단계** | Phase 6 (RED → GREEN → Refactoring) |
| **작성일** | 2026-05-21 |
| **작업 브랜치** | `refactoring` (동기: `A_01` → `a8e02a0`) |
| **작업자** | seoulhan (seoulhanknox@gmail.com) |
| **협업** | Cursor Agent (생성형 AI 보조, 커밋 Co-authored-by 기록) |
| **HEAD 커밋** | `a8e02a0` — `refactor(bce): complete Dual-Track split with UnitRegistry and golden master GREEN` |

---

## 1. 작업 개요

### 1.1 목적

Phase 5 PRD·Phase 6 README 계약에 맞춰 **monolith `UnitConverter.cpp`** 를 **BCE 레이어**(Boundary / Control / Entity / Data)로 분리하고, **Dual-Track TDD**(Domain Track B + Boundary Track A)와 **Golden Master** 회귀 안전장치로 리팩터링을 완료했다.

### 1.2 브랜치·마일스톤 타임라인

| 브랜치 | 역할 | 대표 커밋 |
|--------|------|-----------|
| `spec` / `main` | 문서·거버넌스 | `dacc14b` docs: README/PRD |
| `red` | BCE 골격 + RED 스켈레ton (14 FAIL) | `0c7f22e` test(red): BCE layout, RED Catch2 |
| `green` | TC-A/B GREEN 구현 | `eb2b0ff` feat(green): TC-A-01~07, TC-B-01~07 |
| `refactoring` / `A_01` | Golden Master + 전체 BCE 완성 | `91b4dd7` → `bb8d200` → `a8e02a0` |

### 1.3 산출물 요약

- 실행 파일: `unit_converter`, 테스트: `UnitConverter_test`
- **ctest 21/21 PASS** (로컬 `build`, 2026-05-21 재확인)
- CI 워크플로: `.github/workflows/golden_master.yml` (push/PR)
- 커버리지: gcov 기준 entity·boundary 레이어 수치 측정 (§6)

---

## 2. 완료된 To-Do 항목 요약 (Phase 6 기준)

기준 문서: [docs/TODO.md](../docs/TODO.md) Must-Have / Should-Have, [README.md](../README.md) RED·Golden Master 체크리스트.

### 2.1 Must-Have (🔴) — 완료·부분 완료

| 상태 | 항목 | 비고 |
|------|------|------|
| ☑ | BCE 폴더 골격 (`boundary/`, `control/`, `entity/`, `data/`, `tests/`) | `red` 브랜치에서 생성, `refactoring`에서 정리 |
| ☑ | CMake + C++17 + Catch2 FetchContent | `CMakeLists.txt`, `UnitConverter_test` |
| ☑ | Entity: UnitRegistry + ConversionService (meter 허브) | PRD 비율 `0.3048` / `0.9144` 단일 출처 (`unit_registry.cpp`) |
| ☑ | D-UC01 source 단위 출력 제외 | TC-B-04, GM-TC-01 |
| ☑ | 입력 파싱·검증 (형식·숫자·양수) | TC-A-01~03, 05, 07 |
| ☑ | 미등록 단위 거부 | TC-A-04, exit 3, `Unknown unit: …` |
| ☑ | table 기본 출력 + LHS 보존 | GM-TC-01~03 (target **6자리** 고정 — README 1자리와 문서 갭, §8) |
| ☑ | main/CLI 환산식 제거 → BCE 경유 | `UnitConverter.cpp` thin orchestration |
| ◐ | DomainError ↔ exit ↔ stderr 단일 매핑 | `error_mapper` 구현·일부 테스트; **커버리지 미달 분기** (§6, §7) |
| ◐ | Gherkin 8 Scenario IT 대응 | GM 4시나리오 + TC-A/B 14케이스; **8 Scenario 전부 명시 IT 미완** |
| ◐ | regression_minimum 5건 상시 green | Domain P0 + IT 유사 케이스는 green; **README 5건 명시 체크리스트 미체크** |
| ☑ | 스타터 이관 — monolith → thin wrapper | if-else 제거, `UnitRegistry` 기반 |

### 2.2 Should-Have (🟡) — 선택 완료

| 상태 | 항목 | 비고 |
|------|------|------|
| ☑ | `config/units.json` + Data 로드 | `json_unit_ratio_source`, TC-B-06/07 |
| ◐ | 설정 실패 (corrupt JSON, meter 누락 등) | 기본 fallback(IT-B-07); **실패 3종 IT 미구현** |
| ◐ | FILE_NOT_FOUND 정책 확정 | missing → defaults (TC-B-07); PRD (A/B) **문서 확정 미완** |
| ☑ | `register:unit=meters` 동적 등록 | TC-B-05, Boundary register 테스트 |
| ☑ | duplicate / invalid factor reject | Domain coverage 테스트 |
| ☐ | `--format=csv` / `json` / `xml` 거부 | v1.0 Must 아님, **미구현** |
| ◐ | 커버리지 목표 리포트 | entity·boundary **합산 충족**, 파일별 gap (§6) |
| ☐ | `.clang-format` 전체 적용 | 미실시 |
| ☐ | RED 40 목록 P0 매핑 표 | `docs/RED_TESTS.md`는 `red` 브랜치에만 존재 |

### 2.3 Golden Master 체크리스트 (README)

| ID | 내용 | 상태 |
|----|------|------|
| GM-01~03 | `golden_master_expected.txt` + 시나리오 4종 | ☑ |
| GM-04~06 | `test_golden_master.cpp`, approval helper, CTest `GoldenMaster` | ☑ |
| GM-07 | `.github/workflows/golden_master.yml` | ☑ |
| GM-08 | PR required status check | ☐ (GitHub 설정 수동 필요) |
| GM-09 | Refactoring 후 GM PASS | ☑ (`a8e02a0` 기준 21/21) |

### 2.4 문서·거버넌스 (Done, M0)

| ☑ | 항목 | 완료일 |
|---|------|--------|
| ☑ | Phase 5 PRD, Phase 6 README, `.cursorrules` | 2026-05-20 |
| ☑ | Epic / Gherkin / requirements 패키지 | 2026-05-20 |

---

## 3. RED 단계 결과

### 3.1 RED 커밋·의도

- **커밋:** `0c7f22e` — `test(red): add BCE layout, RED Catch2 skeletons, and QA docs`
- **의도:** 구현 없이(또는 스텁만) **14개 FAIL** 테스트로 계약 고정 (`docs/RED_TESTS.md`, `test_plan.md`, `defect_list.md` 동봉, `red` 브랜치)

### 3.2 작성한 테스트 목록 (RED 스켈레ton, 14건)

**Track A (Boundary / UI)** — `tests/boundary/` 스켈레ton

| Test ID | 요약 | RED 실패 원인 (monolith 대비) |
|---------|------|-------------------------------|
| TC-A-RED-01 | `meter:2.5` → `8.202100 feet` | 포맷·6자리 불일치 (DEF-005/006) |
| TC-A-RED-02 | `meter2.5` → 예외 | `invalid_argument` 없음, exit 1 (DEF-004) |
| TC-A-RED-03 | `meter:-1.0` → 예외 | 음수 환산 진행 (DEF-003) |
| TC-A-RED-04 | `parsec:1.0` → 예외 | unknown 시 exit 3 계약 없음 (DEF-004) |
| TC-A-RED-05 | LHS `2.5 meter =` 보존 | 출력 형식 불일치 |
| TC-A-RED-06 | `feet:1` 교차 환산 | yard 줄 누락/비율 |
| TC-A-RED-07 | `meter:0` / `meter:0.0` | NonPositive 거부 없음 |
| (기타) | stderr / exit 매핑 | 단일 매핑 테이블 부재 |

**Track B (Domain)** — `tests/domain/` 스켈레ton

| Test ID | 요약 | RED 실패 원인 |
|---------|------|---------------|
| TC-B-RED-01 | meter → feet ε | monolith에 Domain API 없음 |
| TC-B-RED-02 | meter → yard ε | 동일 |
| TC-B-RED-03 | feet → meter 역환산 | if-else 중복·테스트 불가 |
| TC-B-RED-04 | source 제외 (D-UC01) | meter target 줄 출력 |
| TC-B-RED-05~07 | Registry / config / register | 레이어 미분리 |

### 3.3 실패 확인 여부

| 확인 항목 | 결과 |
|-----------|------|
| RED 브랜치에서 14 스켈레ton **의도적 FAIL** | ☑ (`0c7f22e` 커밋 메시지·QA 문서) |
| monolith 대비 invariant 위반 목록화 | ☑ `defect_list.md` (red 브랜치) |
| 현재 `refactoring`에서 동일 ID **재실행 RED** | 해당 스켈레ton 파일은 **GREEN용 TC-A/B로 대체**됨 |

---

## 4. GREEN 단계 결과

### 4.1 통과한 테스트 (현재 `refactoring` / `a8e02a0`)

**Domain (Track B)** — `tests/domain/test_track_b.cpp`

| CTest 이름 | 내용 |
|------------|------|
| TC-B-01 | meter → feet (ε 1e-9) |
| TC-B-02 | meter → yard |
| TC-B-03 | feet → meter 역환산 |
| TC-B-04 | convertAll source 제외 (D-UC01) |
| TC-B-05 | register cubit 후 convert |
| TC-B-06 | custom JSON 비율 적용 |
| TC-B-07 | missing config → defaults |
| Domain register duplicate and invalid factor | DuplicateUnit / InvalidFactor |

**Boundary (Track A)** — `tests/boundary/test_track_a.cpp`

| CTest 이름 | 내용 |
|------------|------|
| TC-A-01 | happy path `meter:2.5` |
| TC-A-02 | missing colon |
| TC-A-03 | negative value |
| TC-A-04 | unknown unit (`furlong`) |
| TC-A-05 | invalid number |
| TC-A-06 | output LHS 보존 |
| TC-A-07 | zero value |
| Boundary register command and error mapper | register 파싱, fromCli/fromDomain 일부 |

**Integration (Golden Master)** — `tests/integration/test_golden_master.cpp`

| CTest 이름 | 내용 |
|------------|------|
| GM-TC-01 | `meter:2.5` |
| GM-TC-02 | `feet:1.0` |
| GM-TC-03 | `yard:1.0` |
| GM-TC-04 | `meter:0.0` (실패·stderr) |
| GoldenMaster | `[golden_master]` 태그 일괄 |

**합계:** **21/21 PASS** (2026-05-21, Windows MinGW build)

### 4.2 GREEN 단계 커밋 메시지

```text
feat(green): implement convert meter to feet (Approx 1e-5)
feat(green): implement convert meter to yard (Approx 1e-5)
feat(green): complete Track A/B TC-A-01~07 and TC-B-01~07 with BCE layers
```

(GREEN 작업은 `green` 브랜치 `eb2b0ff` 등에 집약; 이후 `refactoring`에서 GM·레이어 완성.)

---

## 5. Refactoring 결과

### 5.1 선택 항목 (Dual-Track + Golden Master)

| 항목 | 내용 |
|------|------|
| Dual-Track 보호 | Domain `test_track_b` + Boundary `test_track_a` 분리 유지 |
| Golden Master | `tests/golden_master_expected.txt` + `approval_tests.hpp` (capture, diff) |
| UnitRegistry | feet/yard/meter if-else 제거, `meters_per_*` map |
| PRD 비율 단일화 | `3.28084` / `1.09361` 리터럴 제거 → `0.3048`, `0.9144` |
| 출력 정밀도 | target **소수 6자리** (`OutputFormatter`) |
| exit 코드 | 형식/숫자 → 2, unknown unit → 3 |

### 5.2 주요 변경 파일 (`a8e02a0` 기준)

| 레이어 | 추가·변경 파일 |
|--------|----------------|
| Entity | `unit_registry.{hpp,cpp}`, `conversion_service.{hpp,cpp}`, `domain_error.hpp` |
| Control | `convert_use_case`, `register_use_case`, `config_loader` |
| Boundary | `cli_parser`, `output_formatter`, `error_mapper` |
| Data | `json_unit_ratio_source`, `config/units.json` |
| App | `UnitConverter.cpp` (thin main) |
| Test | `test_track_a.cpp`, `test_track_b.cpp`, `test_golden_master.cpp`, `helpers/approval_tests.hpp` |
| CI/Docs | `.github/workflows/golden_master.yml`, `.gitignore`, `README.md` (GM 섹션) |

**제거·대체:** 초기 refactor의 `input_parser`, `conversion_factors.hpp`, 구 Track A/B UI 전용 테스트 파일.

### 5.3 회귀 테스트 통과 여부

| 세트 | 결과 |
|------|------|
| Catch2 전체 (`UnitConverter_test`) | **21/21 PASS** |
| Golden Master (GM-TC-01~04 + aggregate) | **PASS** |
| Dual-Track TC-A-01~07, TC-B-01~07 | **PASS** |
| Refactor 중 GM baseline drift | 초기 **RED** (monolith 4자리 vs 6자리) → baseline·구현 정렬 후 **GREEN** |

### 5.4 Refactoring 커밋 메시지

```text
test: add golden master approval regression harness
test: refactor golden master into per-case GM-TC tests
refactor(bce): complete Dual-Track split with UnitRegistry and golden master GREEN
```

---

## 6. 커버리지 현황 (레이어별 수치)

**측정 방법:** `UnitConverter_test` 실행 후 `build/CMakeFiles/*/*.cpp.gcov` (GNU gcov, Windows MinGW).  
**참고:** `cmake --target coverage`는 Windows 경로 이슈로 실패 가능; lcov 미설치.

### 6.1 Entity (Domain Logic) — 목표 line ≥ 95%

| 파일 | Stmts | Miss | Cover |
|------|------:|-----:|------:|
| `entity/unit_registry.cpp` | 30 | 1 | **96.67%** |
| `entity/conversion_service.cpp` | 18 | 1 | **94.44%** |
| **합산** | **48** | **2** | **95.83%** ☑ |

**미커버 invariant:** `conversion_service.cpp:26` — 루프 내 `!targetMetersPer` 방어 `continue` (정상 Registry 불변식상 unreachable).

### 6.2 Boundary — 목표 line ≥ 85%

| 파일 | Stmts | Miss | Cover |
|------|------:|-----:|------:|
| `boundary/cli_parser.cpp` | 48 | 2 | **95.83%** |
| `boundary/output_formatter.cpp` | 15 | 1 | **93.33%** |
| `boundary/error_mapper.cpp` | 21 | 7 | **66.67%** |
| **합산** | **84** | **10** | **88.10%** ☑ |

**미커버 invariant:** trim 루프(L14/L17), `!isfinite`, `fromCli(UnknownUnit)`, `fromDomain(UnknownUnit|InvalidFactor)`.

### 6.3 Control / Data (참고)

| 레이어 | 상태 |
|--------|------|
| control | TC-B·convert 경로로 주요 UseCase 실행; **별도 gcov 합산 리포트 미첨부** |
| data | TC-B-06/07로 JSON 로드 경로 실행 |

### 6.4 PRD §4.3 대비

| 레이어 | 목표 (line) | 실측 | 판정 |
|--------|-------------|------|------|
| entity | ≥ 95% | 95.83% | ☑ 합산 |
| boundary | ≥ 85% | 88.10% | ☑ 합산 |
| entity (파일) | ≥ 95% | `conversion_service` 94.44% | ◐ 단일 파일 미달 |
| boundary (파일) | ≥ 85% | `error_mapper` 66.67% | ◐ 단일 파일 미달 |

---

## 7. 미완료 항목 및 다음 단계 제안

### 7.1 미완료

| 구분 | 항목 |
|------|------|
| Must-Have | Gherkin **8 Scenario 전부** 명시 IT; `regression_minimum` 5건 README 체크리스트 공식 ☑ |
| Must-Have | `error_mapper` **전 분기** 계약 테스트 (exit 2/3·메시지) |
| Should-Have | csv / json / xml format; corrupt JSON IT 3종; FILE_NOT_FOUND 정책 문서 확정 |
| Should-Have | `.clang-format`; RED 40 ↔ TEST_CASE 추적 표 |
| GM | **GM-08** GitHub required status check |
| 배포 | `git push` / PR `A_01` 원격 반영 (로컬 인증 이슈 시 수동) |
| 커버리지 | TC-A-11~13, TC-B-08~10 **제안만 존재**, 테스트 코드 미추가 |

### 7.2 다음 단계 제안 (우선순위)

1. **TC-A-11~13** — `ErrorMapper::fromCli(UnknownUnit)`, `fromDomain(UnknownUnit|InvalidFactor)` 계약 테스트 추가 → `error_mapper` ≥ 85%.
2. **TC-A-08~09** — trim 공백, `nan`/`inf` 입력 → Boundary invariant 보강.
3. **TC-B-09~10** — `yard`/`feet` 소스 교차 환산 Domain 명시 (GM은 별도 exe라 gcda 미반영 가능).
4. **README ↔ 구현** — 표시 1자리 vs 6자리, exit/stderr 표 **단일화** (RR-2, RR-3).
5. **GM-08** + PR 머지 게이트 — `golden_master.yml` required check.
6. **Should-Have** — `--format=csv|json`, config 실패 IT (M4 마일스톤).

---

## 8. 발견된 이슈 및 해결 방법

| # | 이슈 | 영향 | 해결 |
|---|------|------|------|
| 1 | monolith **exit 1** 고정, unknown 시 exit 3 없음 | RED TC-A-04 | `ErrorMapper` + `unknownUnit()` → exit **3** |
| 2 | 음수·0 허용 | RED TC-A-03/07 | `parsePositiveNumber` + `NonPositiveValue` |
| 3 | if-else 단위 분기·매직넘버 | OCP 위반 | `UnitRegistry` + PRD `meters_per_*` |
| 4 | Golden Master **baseline drift** (4자리 vs 6자리) | Refactor 중 GM RED | `golden_master_expected.txt` PRD 수학 기준 갱신 (`yard` 2.734033 등) |
| 5 | README **1자리** vs 구현 **6자리** | 문서·인수 혼동 | 구현·GM은 6자리; README Quick Start 예시는 아직 1자리 — **문서 동기화 필요** |
| 6 | `error_mapper` 분기 미테스트 | 커버리지 66% | §7.2 TC-A-11~13 제안 |
| 7 | `conversion_service:26` unreachable | gcov 94.44% | 방어 코드 유지 또는 제거/제외 정책 결정 |
| 8 | Windows `cmake --target coverage` gcov 경로 실패 | 자동 리포트 | `gcov -o build/CMakeFiles/.../entity` 수동 또는 CI Linux job |
| 9 | 원격 **push 실패** (인증) | PR 미개설 | 로컬 `git push -u origin refactoring` 후 `gh pr create` |

---

## 9. 생성형 AI 활용 회고

### 9.1 도움이 된 순간

- **RED 테스트 설계:** monolith vs PRD 계약 diff를 Track A/B ID·Given/When/Then·Invariant 표로 구조화 (`docs/RED_TESTS.md` 초안).
- **Dual-Track 분리:** Domain(ε, 비율) vs Boundary(파싱·stderr·exit) 책임 분리 제안 및 파일 트리 스캐폴딩.
- **Golden Master:** approval 패턴(`capture` → `EXPECT_EQ` → unified diff), GM-TC 섹션 파싱, CTest `GoldenMaster` 연동.
- **리팩터링 후 회귀:** GM RED 원인(출력 자릿수·baseline) 진단 및 `golden_master_expected.txt` 정렬.
- **커버리지 감사:** gcov `#####` 라인 ↔ invariant(unknown unit, trim, error_mapper) 매핑 및 **테스트 제안만** 분리 (구현 금지 요청 준수).

### 9.2 한계

- **실행 환경 의존:** Windows MinGW에서 gcov/lcov·CMake `coverage` 타깃이 항상 동작하지 않음 → 수치는 기존 `.gcov` 아티팩트·수동 해석에 의존.
- **도달 불가 분기:** Registry 불변식 하의 `conversion_service:26`은 테스트 제안만으로는 커버 불가 — 설계 판단 필요.
- **문서·코드 불일치:** README 1자리 예시와 6자리 GM이 공존할 때, AI가 “어느 쪽이 계약인지” 사용자 확인 없이 한쪽으로 통일하기 어려움.
- **원격·CI:** GitHub required check(GM-08), push 인증은 IDE 밖 **수동 설정** 필요.

### 9.3 TC 작성 팁 (후속 학습용)

1. **Invariant 먼저:** 비율(0.3048/0.9144), D-UC01(source 제외), 양수만 — RED 문장에 `INV-*` ID를 붙이면 GREEN·Refactor 후에도 추적이 쉽다.
2. **Track 분리:** Domain은 `Approx(1e-9)`만; stderr/exit/trim은 Boundary — 한 TEST_CASE에 섞지 않는다.
3. **Golden Master는 통합 전용:** subprocess `unit_converter`는 `entity_lib` gcda에 안 잡힐 수 있음 → 교차 비율은 **Domain TC-B**로도 한 번 더 고정.
4. **ErrorMapper는 표 기반 테스트:** `fromCli`/`fromDomain`/`unknownUnit` 각각 **exit + message** 한 행씩 — switch default까지 명시.
5. **실패 메시지 고정:** RED에서 `DEF-*`와 기대 stderr 문자열을 적어 두면 GREEN 시 **문자열 일치** 테스트로 바로 전환 가능.

---

## 부록 A — 커버리지 갭 대응 테스트 제안 (미구현)

| ID | 레이어 | 제안 |
|----|--------|------|
| TC-B-08 | Domain | `convertAll("furlong", 1.0)` → `nullopt` |
| TC-B-09 | Domain | `convertAll("yard", 1.0)` → feet 3.0, meter 0.9144 |
| TC-B-10 | Domain | `convertAll("feet", 1.0)` → yard 1/3 |
| TC-A-08 | Boundary | `" meter : 2.5 "` trim |
| TC-A-09 | Boundary | `meter:nan` / `meter:inf` |
| TC-A-11~13 | Boundary | `ErrorMapper` UnknownUnit / InvalidFactor / fromCli(UnknownUnit) |

---

## 부록 B — 참고 경로

| 경로 | 설명 |
|------|------|
| [docs/TODO.md](../docs/TODO.md) | Phase 6 Must/Should/Nice/Tech Debt |
| [docs/PRD.md](../docs/PRD.md) | 인수 게이트 인덱스 |
| [tests/golden_master_expected.txt](../tests/golden_master_expected.txt) | GM baseline |
| `red` 브랜치 `docs/RED_TESTS.md` | RED 14건 상세 (현재 브랜치 미포함) |

---

*본 보고서는 구현 코드 변경 없이 작업 이력·테스트·gcov 결과를 정리한 문서이다.*
