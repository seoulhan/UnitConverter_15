# Dual-Track RED Tests — UnitConverter_15

| 항목 | 내용 |
|------|------|
| **프로젝트** | UnitConverter_15 |
| **현재 코드 가정** | 단일 `main()`, if-else 체인, `std::invalid_argument` 미사용, Boundary/Domain 분리 없음 |
| **단계** | **RED only** — 구현·GREEN·REFACTOR 금지 |
| **비율 계약** | 1 meter = 3.28084 feet · 1 meter = 1.09361 yard |
| **입력 계약** | `std::string` `unit:value` (예: `"meter:2.5"`) |
| **출력 계약 (UI)** | `"2.5 meter = 8.202100 feet"` (LHS 원값·원단위 보존, target full 표기) |
| **실패 예외** | `std::invalid_argument` |
| **추적** | [test_plan.md](../test_plan.md) · [defect_list.md](../defect_list.md) |

> **RED 판정:** 아래 테스트는 현재 `UnitConverter.cpp` 단일 main 기준으로 **컴파일 불가(API 부재)** 또는 **실행 시 assertion/예외 실패**가 정상이다.

---

# UI RED Tests — Test ID / Given/When/Then / Invariant

## Track A — Boundary / UI

### TC-A-RED-01 — 정상 입력 Happy Path

| 필드 | 내용 |
|------|------|
| **Test ID** | `TC-A-RED-01` |
| **테스트 이름** | `test_parse_convert_meter_colon_2p5_returns_conversion_lines` |
| **Given** | 입력 문자열 `"meter:2.5"`; 등록 단위 `meter`, `feet`, `yard`; 비율 1 m = 3.28084 ft, 1 m = 1.09361 yd |
| **When** | Boundary 파서·변환·포맷터가 convert 경로 실행 (stdin/문자열 동일 계약) |
| **Then** | stdout에 `"2.5 meter = 8.202100 feet"` 포함; exit 0; 예외 없음 |
| **Invariant** | **INV-A-01** 정상 `unit:value`는 유한 양수만 허용하고 환산 결과를 반환한다 |
| **RED 실패 이유** | monolith는 파서/반환 타입 없음; 출력이 `"2.5 meter = 8.2021 feet"` 등 **소수 자릿수·포맷 불일치** (DEF-005/006) |

---

### TC-A-RED-02 — 콜론 없음 (잘못된 형식)

| 필드 | 내용 |
|------|------|
| **Test ID** | `TC-A-RED-02` |
| **테스트 이름** | `test_parse_missing_colon_throws_invalid_argument` |
| **Given** | 입력 `"meter2.5"` (콜론 없음) |
| **When** | `parseConvertInput(input)` 호출 |
| **Then** | `std::invalid_argument` throw; 변환 수행 없음 |
| **Invariant** | **INV-A-02** `unit:value` 형식 위반 시 예외로 실패한다 (exit·stderr 매핑은 Control 책임) |
| **RED 실패 이유** | monolith는 `cerr` + `return 1`만 사용, **`invalid_argument` 없음** (DEF-004) |

---

### TC-A-RED-03 — 음수 값

| 필드 | 내용 |
|------|------|
| **Test ID** | `TC-A-RED-03` |
| **테스트 이름** | `test_parse_negative_meter_throws_invalid_argument` |
| **Given** | 입력 `"meter:-1.0"` |
| **When** | `parseConvertInput(input)` 호출 |
| **Then** | `std::invalid_argument` throw |
| **Invariant** | **INV-A-03** value는 **> 0** (README·PRD); 0·음수는 Boundary에서 거부 |
| **RED 실패 이유** | monolith는 `stod` 성공 후 **음수 환산 진행** (DEF-003) |

---

### TC-A-RED-04 — 없는 단위

| 필드 | 내용 |
|------|------|
| **Test ID** | `TC-A-RED-04` |
| **테스트 이름** | `test_parse_unknown_parsec_throws_invalid_argument` |
| **Given** | 입력 `"parsec:1.0"`; Registry에 `parsec` 없음 |
| **When** | 파싱 후 `ensureKnownUnit(registry, unitId)` |
| **Then** | `std::invalid_argument` throw (메시지에 `parsec` 포함 가능) |
| **Invariant** | **INV-A-04** 미등록 `unit_id`는 변환 전에 실패한다 |
| **RED 실패 이유** | monolith는 unknown 시 `cerr` + `return 1`, **예외 타입·exit 3 계약 없음** (DEF-004) |

---

### TC-A-RED-05 — LHS 원입력 보존

| 필드 | 내용 |
|------|------|
| **Test ID** | `TC-A-RED-05` |
| **테스트 이름** | `test_format_table_preserves_source_unit_and_value` |
| **Given** | source `meter`, value `2.5`; target feet 환산값 8.2021… |
| **When** | `formatTable("meter", 2.5, rows)` |
| **Then** | 첫 줄이 `"2.5 meter = 8.202100 feet"` 패턴과 일치 (`2.5`, `meter` LHS 보존) |
| **Invariant** | **INV-A-05** 출력 줄은 `{source_value} {source_unit} = {target_value} {target_unit}`; LHS는 입력 그대로 |
| **RED 실패 이유** | Formatter API 없음; monolith full precision·**6자리 고정 포맷 미구현** |

---

### TC-A-RED-06 — JSON 출력 스키마

| 필드 | 내용 |
|------|------|
| **Test ID** | `TC-A-RED-06` |
| **테스트 이름** | `test_format_json_meter_2p5_matches_schema` |
| **Given** | 입력 `"meter:2.5"`; `--format=json` 또는 동등 Format 옵션 |
| **When** | JSON 직렬화 실행 |
| **Then** | 파싱 가능한 JSON: `source.unit=="meter"`, `source.value==2.5`, `conversions[]`에 `{unit:"feet", value: 8.202100}` 등 (README §6.2 스키마) |
| **Invariant** | **INV-A-06** JSON 성공 응답 스키마 고정; 실패 시 stdout empty + stderr JSON error |
| **RED 실패 이유** | monolith에 **JSON 출력·format 옵션 전무** (Should-Have F-08) |

---

### TC-A-RED-07 — (보조) 영값 경계

| 필드 | 내용 |
|------|------|
| **Test ID** | `TC-A-RED-07` |
| **테스트 이름** | `test_parse_zero_meter_throws_invalid_argument` |
| **Given** | `"meter:0"` |
| **When** | `parseConvertInput` |
| **Then** | `std::invalid_argument` |
| **Invariant** | **INV-A-03** (양수 정책) |
| **RED 실패 이유** | `meter:0` → **0.000000 feet** 출력 (DEF-002) |

---

## Track A — RED 실행 매핑 (Catch2 제안, 미구현 금지)

| Test ID | 제안 `TEST_CASE` 태그 | 현재 monolith 대비 |
|---------|----------------------|-------------------|
| TC-A-RED-01 | `[boundary][red][happy]` | FAIL (포맷·API) |
| TC-A-RED-02 | `[boundary][red][format]` | FAIL (예외 없음) |
| TC-A-RED-03 | `[boundary][red][negative]` | FAIL |
| TC-A-RED-04 | `[boundary][red][unknown]` | FAIL |
| TC-A-RED-05 | `[boundary][red][format]` | FAIL |
| TC-A-RED-06 | `[boundary][red][json]` | FAIL (기능 없음) |
| TC-A-RED-07 | `[boundary][red][zero]` | FAIL |

---

# Logic RED Tests — Test ID / Scenario / Invariant

## Track B — Domain / Logic

### TC-B-RED-01 — meter → feet (ε 1e-5)

| 필드 | 내용 |
|------|------|
| **Test ID** | `TC-B-RED-01` |
| **Scenario** | `convert("meter", 2.5, "feet")` 호출 |
| **Given** | Registry 기본 비율; **1 meter = 3.28084 feet** |
| **When** | `ConversionService::convert` 실행 |
| **Then** | 결과 `8.20210` (또는 8.2021), `\|actual − 2.5×3.28084\| ≤ 1e-5` |
| **Invariant** | **D-INV-1** `v_B = (v_A × R_A) / R_B`, `R_meter=1.0`, `R_feet=0.3048` |
| **RED 실패 설명** | monolith에 `convert(from,to)` API 없음; 테스트는 **Entity 단위** 대상—main() 직접 호출 불가. 분리 전 RED는 **링크/심볼 부재로 컴파일 실패** 의도 |

---

### TC-B-RED-02 — meter → yard (ε 1e-5)

| 필드 | 내용 |
|------|------|
| **Test ID** | `TC-B-RED-02` |
| **Scenario** | `convert("meter", 1.0, "yard")` |
| **Given** | **1 meter = 1.09361 yard** |
| **When** | `convert` 실행 |
| **Then** | `≈ 1.09361`, margin `1e-5` |
| **Invariant** | **D-INV-1** |
| **RED 실패 설명** | TC-B-RED-01과 동일—Domain 서비스 미노출 |

---

### TC-B-RED-03 — feet → meter 역변환

| 필드 | 내용 |
|------|------|
| **Test ID** | `TC-B-RED-03` |
| **Scenario** | `convert("feet", 1.0, "meter")` |
| **Given** | 1 foot = 0.3048 meter |
| **When** | `convert` 실행 |
| **Then** | `≈ 0.30480`, margin `1e-5` |
| **Invariant** | **D-INV-2** 역변환은 동일 공식으로 대칭 |
| **RED 실패 설명** | if-else는 feet→meter 분기 있으나 **함수 단위 검증 불가**; golden을 main에 embedded literal로만 검증 |

---

### TC-B-RED-04 — convertAll + source 제외

| 필드 | 내용 |
|------|------|
| **Test ID** | `TC-B-RED-04` |
| **Scenario** | `convertAll("meter", 1.0)` |
| **Given** | 등록 단위 `meter`, `feet`, `yard` |
| **When** | `convertAll` 실행 |
| **Then** | 결과에 `feet`, `yard` 포함; **`meter` target 없음**; 각 값 ε 1e-5 |
| **Invariant** | **D-UC01** source 단위는 convert-all 출력에서 제외 |
| **RED 실패 설명** | monolith는 **meter를 target 줄로 출력** (`= … meter` 줄 존재, DEF-005) |

---

### TC-B-RED-05 — registerUnit 후 변환

| 필드 | 내용 |
|------|------|
| **Test ID** | `TC-B-RED-05` |
| **Scenario** | `registerUnit("cubit", 0.4572)` 후 `convert("meter", 1.0, "cubit")` |
| **Given** | 1 cubit = 0.4572 meter |
| **When** | 불변 Registry에 단위 추가 후 convert |
| **Then** | `≈ 1/0.4572`, ε 1e-5; duplicate 등록 시 throw |
| **Invariant** | **D-INV-3** OCP—새 단위는 등록으로만 확장, if-else 체인 변경 없음 |
| **RED 실패 설명** | `registerUnit` / `UnitRegistry` **API 없음**; 동적 등록 경로 없음 |

---

### TC-B-RED-06 — loadConfig 유효 JSON/YAML

| 필드 | 내용 |
|------|------|
| **Test ID** | `TC-B-RED-06` |
| **Scenario** | `loadConfig("config/units.json")` 성공 |
| **Given** | `feet: 0.3048`, `yard: 0.9144`, `meter: 1.0` |
| **When** | Data 레이어 로드 → Registry 반영 |
| **Then** | `convert("meter", 2.5, "feet")`가 파일 비율과 일치 |
| **Invariant** | **D-INV-4** 비율 단일 출처(JSON/YAML); main에 literal 금지 |
| **RED 실패 설명** | **파일 로드·IUnitRatioSource 없음**; 비율이 `UnitConverter.cpp` if-else literal (DEF-008) |

---

### TC-B-RED-07 — loadConfig 무효 경로 → 기본값

| 필드 | 내용 |
|------|------|
| **Test ID** | `TC-B-RED-07` |
| **Scenario** | `loadConfig("config/not_exist.json")` |
| **Given** | 내장 기본: 3.28084 / 1.09361 등가 |
| **When** | 로드 실패 |
| **Then** | Registry 기본값 유지; `convert("meter", 1.0, "feet") ≈ 3.28084` |
| **Invariant** | **D-INV-5** 설정 실패 시 안전 기본 스냅샷 |
| **RED 실패 설명** | loadConfig API 없음; fallback 정책 **미정의** |

---

## Track B — RED 실행 매핑

| Test ID | 제안 `TEST_CASE` | 보호 Invariant | monolith RED 실패 모드 |
|---------|------------------|----------------|------------------------|
| TC-B-RED-01 | `test_convert_meter_to_feet_within_1e5` | D-INV-1 | API/compile FAIL |
| TC-B-RED-02 | `test_convert_meter_to_yard_within_1e5` | D-INV-1 | API/compile FAIL |
| TC-B-RED-03 | `test_convert_feet_to_meter_reverse` | D-INV-2 | API/compile FAIL |
| TC-B-RED-04 | `test_convert_all_excludes_source_meter` | D-UC01 | ASSERT: meter in output |
| TC-B-RED-05 | `test_register_cubit_then_convert` | D-INV-3 | API/compile FAIL |
| TC-B-RED-06 | `test_load_config_json_applies_ratios` | D-INV-4 | API/compile FAIL |
| TC-B-RED-07 | `test_load_config_missing_keeps_defaults` | D-INV-5 | API/compile FAIL |

---

## Dual-Track 의존 순서 (RED only)

```text
Track B (Domain) RED  →  TC-B-RED-01~07  (순수 환산·Registry·Config)
Track A (UI) RED      →  TC-A-RED-01~07  (Parser/Formatter/JSON; Domain Mock 또는 Facade)
```

- Track A-RED-01은 Track B-RED-01 golden에 **의존** (8.202100 표시 vs 8.2021 내부값).
- **금지:** RED 단계에서 main if-else 수정, GREEN 맞추기 위한 assertion 완화.

---

## RED 완료 체크 (테스트 코드 작성 시)

| 체크 | 조건 |
|------|------|
| ☐ | `ctest` / Catch2 실행 시 위 ID **전부 FAIL** (빈 `REQUIRE(true)` 금지) |
| ☐ | FAIL 메시지에 expected vs actual 명시 (예: expected `8.202100`, got `8.2021`) |
| ☐ | 구현(production) `.cpp` diff **0** |
| ☐ | [README RED To-Do](../README.md#red-단계-to-do-리스트) 항목과 Test ID 1:1 매핑 |

---

*문서 only — RED 명세. 구현·GREEN·REFACTOR는 다음 단계.*
