# UnitConverter — 테스트 계획서 (Test Plan)

| 항목 | 내용 |
|------|------|
| **문서 버전** | 1.0 |
| **기준 샘플** | meter → feet 변환 (`meter:2.5`) |
| **기술 스택** | C++17, CMake, Catch2 |
| **계약 출처** | [README.md](README.md), [docs/PRD.md](docs/PRD.md), [.cursorrules](.cursorrules) |
| **작성 관점** | 시니어 QA 리드 — BCE 레이어·TDD·회귀 최소 세트 정렬 |

---

## 1. 목적 및 범위

### 1.1 목적

본 계획서는 **meter 허브 기반 길이 변환**의 핵심 샘플(`meter:2.5` → feet)을 중심으로, Catch2 기반 **단위·통합 테스트 범위**, **경계·예외 케이스**, **레이어별 커버리지 목표**, **gcov/lcov 측정 전략**을 정의한다.

### 1.2 In Scope

| 영역 | 내용 |
|------|------|
| Domain (Entity) | `ConversionService`, `UnitRegistry`, meter 환산식, D-UC01(source 제외), ε golden |
| Boundary | `CliParser`, `OutputFormatter`(table), `ErrorMapper`, exit code·stderr 계약 |
| Control | `ConvertUseCase`, DomainError → exit/stderr 매핑 |
| Data | `config/units.json` 로드(Should-Have, M4) |
| Integration | CLI E2E: stdin/argv → stdout/stderr/exit |

### 1.3 Out of Scope (v1.0)

- `--format=csv|json` 상세 스키마(Should-Have, 별도 테스트 스위트)
- `register:` 동적 등록 전용 시나리오(회귀 보조)
- 네트워크·다국어·GUI

### 1.4 샘플 예제 (Traceability Anchor)

| 항목 | 값 |
|------|-----|
| 기능 | meter → feet 변환 |
| 입력 | `meter:2.5` |
| Domain golden | `2.5 × 3.28084 = 8.2021` feet, `|actual − expected| ≤ 1e-9` |
| IT table (stdout) | `2.5 meter = 8.2 feet` (target만 소수 1자리) |
| 연관 yard 줄 | `2.5 meter = 2.7 yard` (`2.5 × 1.09361` → 표시 2.7) |
| README 추적 | 입력 형식 계약 정상 #1, 출력 포맷 table, 회귀 최소 세트 IT `meter:2.5` |

---

## 2. Catch2 테스트 범위 및 우선순위

### 2.1 디렉터리·네이밍

```
tests/
├── domain/          # Domain_*  — Entity 순수 로직
├── boundary/        # Boundary_* — 파서·포맷터·에러 매핑
├── data/            # Data_* — JSON 스냅샷
└── integration/     # IT_* — CLI E2E
```

- 패턴: **AAA** (Arrange–Act–Assert)
- 프레임워크: **Catch2 고정** (`.cursorrules` `framework_fixed: true`)
- TDD 순서: **Domain RED → Data → Boundary → Integration**

### 2.2 우선순위 매트릭스

| 우선순위 | 레이어 | 대표 TEST_CASE | 샘플 연관 | 선행 조건 |
|----------|--------|----------------|-----------|-----------|
| **P0** | Domain | `Domain_convert_meter_to_feet_golden_2p5` | `2.5 m→ft` ε golden | 없음 |
| **P0** | Domain | `Domain_convert_all_excludes_source` | D-UC01: table에 `meter` target 없음 | P0 golden |
| **P0** | Integration | `IT_OK_01_meter_colon_2p5_table` | 회귀 최소 #2 | Domain P0 green |
| **P1** | Boundary | `Boundary_parser_rejects_non_positive` | `meter:0`, `meter:-1.5` | Domain Mock |
| **P1** | Boundary | `Boundary_parser_invalid_number` | `meter:abc` | — |
| **P1** | Boundary | `Boundary_parser_missing_colon` | `meter2.5` | — |
| **P1** | Integration | `IT_FAIL_01_negative`, `IT_FAIL_02_format`, `IT_FAIL_03_unknown` | 회귀 #3–5 | P1 Boundary |
| **P2** | Domain | `Domain_convert_large_value_no_overflow` | 매우 큰 수 | P0 |
| **P2** | Boundary | `Boundary_formatter_one_decimal_feet` | `8.2` vs `8.2021` 분리 | P0 |
| **P2** | Control | `Control_error_mapper_exit_stderr` | exit 2/3·문구 일치 | P1 |
| **P3** | Data | `Data_load_units_json_registry` | `feet: 0.3048` | M4 |
| **P3** | Integration | `IT_unknown_unit_parsec` | `parsec:1.0` (계약 동일) | P1 |

### 2.3 레이어별 단위 테스트 범위

#### Domain (Entity) — 최우선

| ID | 범위 | Assert 요약 |
|----|------|-------------|
| D-01 | `ConversionService::convert(meter, 2.5, feet)` | `≈ 8.2021`, ε `1e-9` |
| D-02 | `convertAll(meter, 2.5)` | 결과에 `feet`, `yard` 포함; **`meter` target 없음** |
| D-03 | 환산식 `v_B = (v_A × R_A) / R_B` | `R_feet=0.3048`, `R_meter=1.0` |
| D-04 | `PositiveLength` / 값 검증 | `≤0`, non-finite → `DomainError` (예외 아님) |
| D-05 | Registry 불변 | unknown `UnitId` → 실패 |

#### Boundary

| ID | 범위 | Assert 요약 |
|----|------|-------------|
| B-01 | `CliParser::parse("meter:2.5")` | unit=`meter`, value=`2.5` |
| B-02 | `OutputFormatter::table` | 줄 `2.5 meter = 8.2 feet` (반올림만 Boundary) |
| B-03 | stderr·exit | README 고정 문구·코드 (§3.2) |
| B-04 | `unit_id` trim·regex | 앞뒤 공백, `[a-z][a-z0-9_]{0,31}` |

#### Control

| ID | 범위 | Assert 요약 |
|----|------|-------------|
| C-01 | `ConvertUseCase` 성공 경로 | Formatter 입력 DTO 정확 |
| C-02 | `ErrorMapper` | DomainError ↔ exit 2/3 ↔ stderr 단일 테이블 |

#### Data (Should-Have)

| ID | 범위 | Assert 요약 |
|----|------|-------------|
| DT-01 | `units.json` 로드 | Registry와 `feet`/`yard` 비율 일치 |

### 2.4 회귀 최소 세트 (상시 Green, 5건)

README·`.cursorrules` `regression_minimum`과 동기:

1. Domain P0 — convert + length validation  
2. `IT_OK_01` — `meter:2.5` table (feet·yard 줄, source 제외)  
3. `IT_FAIL_01` — 음수 (`meter:-1.5`)  
4. `IT_FAIL_02` — 형식 (`meter2.5`)  
5. `IT_FAIL_03` — unknown unit (`furlong:1` 또는 동등 `parsec:1.0`)

---

## 3. 경계값 케이스 목록

계약: **value는 유한 십진수이며 반드시 > 0**. 실패 시 **stdout 변환 줄 0**, exit·stderr는 아래 표준.

| # | 케이스 ID | 입력 | 기대 동작 | exit | stderr 패턴 (요약) | 주 테스트 레이어 |
|---|-----------|------|-----------|------|-------------------|------------------|
| BV-01 | 영값 | `meter:0` | 거부, 변환 없음 | **2** | `Value must be positive: 0` | Boundary P1, IT |
| BV-02 | 음수 | `meter:-1.5` | 거부 | **2** | `Value must be positive: -1.5` | Boundary, IT-FAIL-01 |
| BV-03 | 매우 큰 수 | `meter:1e308` 또는 `meter:999999999999` | (1) 파싱 성공 시 Domain 환산 finite (2) overflow/inf 시 거부 | 0 또는 **2** | non-finite 시 `Invalid number` 또는 Domain 거부 | Domain P2, Boundary |
| BV-04 | 소수 파싱 실패 | `meter:abc` | 거부 | **2** | `Invalid number: abc` | Boundary, IT |
| BV-05 | 이중 소수점 | `yard:1.2.3` | 거부 | **2** | invalid number | Boundary |
| BV-06 | 콜론 없음 | `meter2.5` | 거부 | **2** | `Invalid format. Use unit:value (ex: meter:2.5)` | Boundary, IT-FAIL-02 |
| BV-07 | 미등록 단위 | `parsec:1.0` | 거부 (README 예: `furlong:1` 동일 클래스) | **3** | `Unknown unit: parsec` | Integration P1 |
| BV-08 | 샘플 정상 | `meter:2.5` | feet·yard table, LHS 보존 | **0** | (없음) | Domain P0, IT-OK-01 |
| BV-09 | 최소 양수 | `meter:0.0000001` | ε golden 환산 | **0** | — | Domain P2 |
| BV-10 | 공백 trim | ` meter : 2.5 ` | parse 성공 후 동일 golden | **0** | — | Boundary |

### 3.1 BV-03 (오버플로) 상세 시나리오

| 단계 | Arrange | Act | Assert |
|------|---------|-----|--------|
| A | Registry 기본 3단위 | `convert(meter, 1e100, feet)` | `std::isfinite(result)` |
| B | Parser | `meter:1e309` (플랫폼에 따라 inf) | exit 2 또는 Domain non-finite 거부 |
| C | Golden | `meter:2.5` | **항상** 8.2021 — 회귀 앵커 유지 |

> **QA 메모:** 레거시 `UnitConverter.cpp`는 exit `1`·source `meter` 출력 포함 등 계약과 불일치. 테스트 기준은 **README/PRD**이며, IT는 `boundary/cli_main` 대상으로 작성한다.

### 3.2 meter→feet 경계값 (샘플 확장)

| 입력 value | Domain feet (full) | Table target (1자리) |
|------------|-------------------|----------------------|
| `0.0000001` | `3.28084e-7` | `0.0 feet` (반올림 정책 확인) |
| `2.5` | `8.2021` | `8.2 feet` |
| `999999.0` | finite golden | Formatter overflow 없음 |

---

## 4. 예외·특이 케이스 목록

Domain은 **예외 기반 흐름 제어 금지** (`.cursorrules`); `Result` / `std::optional` / `DomainError` enum 사용.

| # | ID | 설명 | 입력/조건 | 기대 | 레이어 |
|---|-----|------|-----------|------|--------|
| EX-01 | non-finite | `meter:inf`, `meter:nan` | 거부, exit 2 | Boundary |
| EX-02 | empty stdin | EOF 즉시 | exit 2, stdout 0 | Integration |
| EX-03 | empty value | `meter:` | exit 2, invalid number | Boundary |
| EX-04 | register 혼입 | `register:cubit` (형식 오류) | exit 2, register stderr | Boundary |
| EX-05 | duplicate unit | `register:feet=0.3` | 거부, Registry 불변 | Control/Data |
| EX-06 | config corrupt | JSON 파손 | exit ≠ 0, stdout 0 | Data IT |
| EX-07 | D-UC01 위반 | 성공 출력에 `meter` target 줄 | **버그** — IT 실패 | Integration |
| EX-08 | TD-4 혼동 | Domain에 `8.2` golden 사용 | **금지** — Boundary만 | Domain negative test |
| EX-09 | magic number | Entity에 literal `3.28084` | **금지** — config/constexpr 상수 | Static review + forbidden |
| EX-10 | `--format=xml` | 잘못된 format | exit 2 | Boundary (Should) |

### 4.1 stderr·exit 단일 매핑 (RR-2)

| DomainError / 실패 유형 | exit | stderr (human) |
|-------------------------|------|----------------|
| 형식·콜론·register·format | 2 | README §입력 형식 표 |
| 미등록 단위 | 3 | `Unknown unit: <id>` |
| 비양수·invalid number | 2 | `Value must be positive: …` / `Invalid number: …` |

모든 매핑은 `Control_error_mapper_*` **한 곳**에서 테스트한다.

---

## 5. 커버리지 목표

PRD §4.3 · README · `.cursorrules` `coverage_targets` 기준.

| 레이어 | Line | Branch | 샘플 예제 기여도 |
|--------|------|--------|------------------|
| **entity (Domain)** | **≥ 95%** | **≥ 90%** | `ConversionService` meter→feet, D-UC01, PositiveLength |
| control | ≥ 90% | ≥ 85% | `ConvertUseCase` 성공/실패 매핑 |
| **boundary** | **≥ 85%** | **≥ 80%** | Parser, Formatter 1자리, ErrorMapper |
| data | ≥ 90% | ≥ 85% | JSON load (Should) |

### 5.1 샘플 예제로 달성해야 할 최소 라인

- `entity/conversion_service.cpp` — `convert`, `convertAll` 분기  
- `boundary/cli_parser.cpp` — `unit:value` 분리, `stod` 실패 경로  
- `boundary/output_formatter.cpp` — table 1자리  
- `control/convert_use_case.cpp` — orchestration 전 경로  

### 5.2 커버리지에서 제외·별도 보고

| 대상 | 정책 |
|------|------|
| `UnitConverter.cpp` (레거시) | 마이그레이션 후 **삭제**; v1 커버리지 KPI에 **포함하지 않음** |
| Catch2 `tests/**` | 측정 제외 |
| third_party Catch2 | 제외 |

---

## 6. gcov / lcov 측정 전략

### 6.1 목표

- **Domain `entity/`**: line **≥ 95%** (샘플: `meter:2.5` → feet 경로 필수 hit)  
- **Boundary `boundary/`**: line **≥ 85%** (Parser 실패 경로 + Formatter)  
- 레거시 **`UnitConverter.cpp`**: 참고용 별도 리포트만 (KPI 미포함)

### 6.2 CMake 빌드 플래그 (gcc/g++)

```bash
# 커버리지 빌드 디렉터리 (Debug 권장)
cmake -B build-cov -S . \
  -DCMAKE_BUILD_TYPE=Debug \
  -DCMAKE_CXX_FLAGS="--coverage -O0 -g" \
  -DCMAKE_EXE_LINKER_FLAGS="--coverage"

cmake --build build-cov
./build-cov/unit_converter_tests
```

> Clang: `--coverage` 대신 `-fprofile-instr-generate -fcoverage-mapping` + `llvm-cov report` (동일 KPI, 도구만 상이).

### 6.3 gcov 원시 데이터

```bash
# 테스트 실행 후 .gcda 생성 (같은 build-cov 트리에서)
cd build-cov
gcov -b -c -o . ../entity/conversion_service.cpp
gcov -b -c -o . ../boundary/cli_parser.cpp
```

- `-b`: branch coverage  
- 소스 경로는 CMake이 설정한 `CMAKE_CXX_FLAGS`와 동일 빌드 트리 기준  

### 6.4 lcov HTML 리포트 (권장 제출물)

```bash
# 1) 캡처 (테스트 실행 직후)
lcov --capture --directory build-cov --output-file coverage.info \
  --rc lcov_branch_coverage=1

# 2) 노이즈 제거
lcov --remove coverage.info \
  '/usr/*' '*/tests/*' '*/catch2/*' '*/build-cov/*' \
  --output-file coverage.filtered.info

# 3) 레이어별 리포트
genhtml coverage.filtered.info --output-directory coverage-html \
  --branch-coverage --legend

# 4) KPI 게이트 (예: Domain)
lcov --summary coverage.filtered.info --rc lcov_branch_coverage=1 | \
  grep -E 'entity|boundary'
```

### 6.5 `UnitConverter.cpp` 별도 측정 (레거시 추적)

마이그레이션 전후 비교용 **비-KPI** 스위트:

```bash
# 레거시 단독 바이너리가 있다면
g++ --coverage -O0 -g -o build-cov/unit_converter_legacy UnitConverter.cpp
./build-cov/unit_converter_legacy <<< "meter:2.5"
gcov -b -c UnitConverter.cpp

lcov --capture --directory . --output-file legacy.info
lcov --extract legacy.info '*UnitConverter.cpp' --output-file legacy-only.info
genhtml legacy-only.info --output-directory coverage-legacy-html
```

| 관점 | `UnitConverter.cpp` | BCE `entity/` + `boundary/` |
|------|----------------------|-------------------------------|
| KPI | ✗ (참고만) | ✓ PRD §4.3 |
| 샘플 `meter:2.5` | 수동 stdin, exit 1 등 불일치 | IT-OK-01 계약 |
| 마이그레이션 | `boundary/cli_main.cpp` 이전 후 삭제 | 단일 출처 |

### 6.6 CI·PR 게이트

| 게이트 | 조건 |
|--------|------|
| G-1 | `ctest --test-dir build-cov` — regression_minimum **5/5 green** |
| G-2 | lcov `entity/` line ≥ **95%** |
| G-3 | lcov `boundary/` line ≥ **85%** |
| G-4 | PR에 `coverage-html/index.html` 또는 lcov summary 스크린샷 첨부 |
| G-5 | `UnitConverter.cpp` line 상승 시 **감소 계획** PR 설명 필수 |

### 6.7 샘플 예제 커버리지 체크리스트

- [ ] `ConversionService::convert` meter→feet 라인 hit (gcov)  
- [ ] `convertAll`에서 source 제외 분기 hit  
- [ ] `CliParser` 성공 경로 `meter:2.5` hit  
- [ ] `OutputFormatter` 반올림 분기 hit  
- [ ] 실패 경로: BV-01~07 각각 별도 TEST_CASE  

---

## 7. 테스트 데이터·Golden 규칙

| 규칙 ID | 내용 |
|---------|------|
| RR-1 | 비율 golden은 `meters_per_unit`(JSON) 또는 Entity constexpr **단일 출처** |
| RR-2 | Domain: `2.5 × 3.28084`, ε=`1e-9`; Boundary 표시: `8.2` only |
| RR-3 | README 예시 `furlong` ↔ 계획 `parsec` — **동일 exit 3·Unknown unit 패턴** |
| RR-4 | golden·stderr·exit 변경 시 Domain + Boundary + IT **동시** 갱신 |

---

## 8. 일정·마일스톤 (QA 관점)

| 마일스톤 | 산출물 | 샘플 예제 검증 |
|----------|--------|----------------|
| M1 Domain green | `tests/domain/*` P0 | `Domain_convert_meter_to_feet_golden_2p5` |
| M2 Boundary+IT | `tests/boundary/*`, `tests/integration/*` | IT-OK-01, IT-FAIL 01–03 |
| M3 Coverage gate | lcov 리포트 | entity ≥95%, boundary ≥85% |
| M4 Legacy 제거 | `UnitConverter.cpp` 삭제 | legacy gcov 리포트 종료 |

---

## 9. 부록 — TEST_CASE 초안 목록

```
Domain_convert_meter_to_feet_golden_2p5
Domain_convert_all_excludes_source
Domain_rejects_non_positive_length
Domain_convert_large_finite_value

Boundary_parser_meter_colon_2p5
Boundary_parser_rejects_meter_colon_zero
Boundary_parser_rejects_meter_colon_negative
Boundary_parser_rejects_meter_colon_abc
Boundary_parser_rejects_meter2p5_no_colon
Boundary_formatter_table_one_decimal_feet

IT_OK_01_meter_colon_2p5_table
IT_FAIL_01_meter_colon_negative
IT_FAIL_02_meter2p5_invalid_format
IT_FAIL_03_parsec_unknown_unit
```

---

## 10. 문서 이력

| 버전 | 날짜 | 변경 |
|------|------|------|
| 1.0 | 2026-05-21 | 최초 작성 — 샘플 `meter→feet`, Catch2·경계·커버리지·gcov/lcov |

*본 문서는 구현 상태([docs/TODO.md](docs/TODO.md))와 독립이며, 계약 변경 시 README·PRD와 함께 갱신한다.*
