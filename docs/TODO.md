# To-Do 리스트 — UnitConverter (C++)

**워크스페이스:** `c:\DEV\UnitConverter_15`  
**기준:** Phase 5 PRD — §3 기능 요구, §7.1 인수 기준, §7.2 회귀 보호(RR)  
**검증 주체:** **학습자(Alex)** 구현·테스트 실행, **동료 리뷰어** 계약·레이어 점검, **강사** 인수 게이트 확인

---

## 🔴 필수 (Must-Have) — v1.0 릴리스 차단 항목

| ☐ | 작업 설명 | 연관 PRD | 완료 기준 (누가 → 무엇 → 통과) |
|---|-----------|----------|----------------------------------|
| [ ] | **BCE 폴더 골격** 생성: `boundary/`, `control/`, `entity/`, `data/`, `tests/{domain,boundary,data,integration}` | §4.2, file_structure | **Alex**가 트리 생성 → **동료**가 README 아키텍처와 일치 확인 → `.cursorrules` `file_structure.tree`와 diff 0 |
| [ ] | **CMake + C++17** 빌드: 앱 `unit_converter`, 테스트 `unit_converter_tests` | §4.1, README Quick Start | **Alex**가 `cmake --build` 성공 → **Alex**가 `./build/unit_converter` 실행 → exit 0 대기 가능 |
| [ ] | **Catch2 도입** (framework_fixed) | §4.1, F-06, US-07 | **Alex**가 `ctest --test-dir build` 실행 → 최소 1 TEST_CASE 컴파일·실행됨 |
| [ ] | **Entity: UnitRegistry + ConversionService** — meter 허브 환산 | F-01, F-03, US-03, SC-4, G-1 | **Alex**가 Domain 테스트 추가 → `2.5 m→ft` ε≤1e-9 **green**; **동료** golden 출처 단일 확인 |
| [ ] | **D-UC01** source 단위 출력 제외 | F-01, §3.3.6, G-1 | **Alex**가 `Domain_convert_all_excludes_source` green → `meter:2.5` table에 `meter` target 줄 **없음** |
| [ ] | **입력 파싱·검증** (형식·숫자·양수·trim) | F-02, US-01, G-3~7 | **Alex**가 Boundary/IT 6케이스 green → exit 2/3·stderr 패턴 **문자열 일치** |
| [ ] | **미등록 단위** 거부 | F-05, US-02, G-8 | **Alex**가 `furlong:1` IT green → exit **3**, `Unknown unit: furlong`, stdout empty |
| [ ] | **table 기본 출력** + LHS 보존 + target 1자리 | F-04, §6.1, G-1, G-2 | **Alex**가 IT `meter:2.5`, `feet:1` green → 줄이 `2.5 meter =` / `1 feet =`로 시작 |
| [ ] | **main/CLI 변환 로직 제거** — Boundary→Control→Entity만 | F-01, tdd_rules, forbidden | **동료**가 `main`/`cli_parser`에 환산식 0건 리뷰 → **Alex** forbidden 위반 0 |
| [ ] | **DomainError ↔ exit ↔ stderr** 단일 매핑 테이블 | F-02, F-05, RR-2 | **Alex**가 `error_mapper` 문서·테스트 1곳 → 형식/unknown/비양수 AC 전부 매핑 테스트 green |
| [ ] | **Gherkin 8 Scenario** IT 대응 | AC-1, G-1~8 | **Alex**가 8 Scenario step 또는 동등 IT **전부 green** |
| [ ] | **regression_minimum 5건** 상시 green | RR-6, SC-5, US-07 | **Alex**가 README 회귀 목록 5건 ctest green → **강사** 체크리스트 ☑ |
| [ ] | **스타터 이관** — `UnitConverter.cpp` 로직 분리 후 역할 제한 | file_structure, Tech Debt TD-1 | **Alex**가 monolith 삭제 또는 thin wrapper만 유지 → **동료** boundary `cli_main`만 entry 확인 |

---

## 🟡 권장 (Should-Have) — 품질 향상 항목

| ☐ | 작업 설명 | 연관 PRD | 완료 기준 (누가 → 무엇 → 통과) |
|---|-----------|----------|----------------------------------|
| [ ] | **`config/units.json` v1** + Data 로드 | F-09, F-11, US-05, §5.2 | **Alex**가 valid JSON 로드 IT green → Registry가 파일과 일치 |
| [ ] | **설정 실패** (corrupt JSON, meter 누락, factor≤0) | F-11, US-05, G-8(유사) | **Alex**가 실패 IT 3종 green → exit≠0, stdout 변환 줄 0 |
| [ ] | **FILE_NOT_FOUND 정책 1종 확정** 및 테스트 고정 | F-11, PRD 갭 | **Alex**가 README·PRD에 (A) fallback 또는 (B) 종료 **하나** 명시 → 해당 IT green |
| [ ] | **`register:unit=meters`** 동적 등록 | F-10, US-06, SC-7 | **Alex**가 `register:cubit=0.4572` 후 convert IT green → cubit target 포함 |
| [ ] | **duplicate 등록 reject** | F-12, US-02 | **Alex**가 `feet` 재등록 테스트 green → Registry 불변 |
| [ ] | **`--format=csv`** | F-07, US-04 | **Alex**가 헤더·행 수·1자리 AC 테스트 green |
| [ ] | **`--format=json`** 성공·실패 스키마 | F-08, F-15, US-04 | **Alex**가 성공 JSON parse + 실패 시 stdout empty IT green |
| [ ] | **`--format=xml` 거부** | F-13, US-04 | **Alex**가 exit 2 + 고정 stderr green |
| [ ] | **커버리지 목표** 달성 리포트 | §4.3, SC-1, SC-2, G-4 | **Alex**가 gcov/llvm-cov 제출 → entity line ≥95%, boundary ≥85% 등 **수치 충족** |
| [ ] | **`.clang-format`** 적용 | code_style | **Alex**가 `clang-format` 전체 통과 → **동료** 스타일 리뷰 1회 |
| [ ] | **RED 40 목록** P0+Boundary 직렬화 매핑 | SC-3, Phase 4 RED | **Alex**가 TEST_CASE 제목↔ invariant 표 작성 → 미구현 0건 또는 ☑ 사유 |

---

## 🟢 선택 (Nice-to-Have) — v2.0 후보

| ☐ | 작업 설명 | 기대 가치 |
|---|-----------|-----------|
| [ ] | **YAML 설정** (JSON 동등 스키마) | F-14 — 설정 포맷 선택 유연성 |
| [ ] | **JSON 모드 human stderr 병행 정책** 문서·테스트 단일화 | F-15 — 디버깅·로그 일관성 |
| [ ] | **Gherkin Scenario: register + cubit** 추가 | US-06 ↔ BDD 추적 공백 해소 |
| [ ] | **BDD 러너** (Cucumber/cpp 또는 수동 step 테이블) | 비개발자 인수 시나리오 가독성 |
| [ ] | **CI 워크플로** (로컬 스크립트 수준) | regression_minimum 자동 실행 |
| [ ] | **config hot-reload** | 운영 중 비율 변경 (학습 범위 초과 시 v2) |

---

## 🔵 기술 부채 (Tech Debt)

| ☐ | 문제 설명 | 발생 원인 | 해결 방향 |
|---|-----------|-----------|-----------|
| [ ] | **TD-1 Monolith** `UnitConverter.cpp`에 파싱·환산·출력 혼재 | 스타터 단일 파일 제공 | boundary/cli + entity 이관 후 삭제 (Must-Have 마지막 항목) |
| [ ] | **TD-2 README vs 빌드 불일치** CMake/Catch2 문서화, 빌드 파일 없음 | Phase 6 README 선행 작성 | CMakeLists + Catch2 FetchContent (Must-Have) |
| [ ] | **TD-3 비율 이중 표기** README 3.28084 vs `meters_per_feet` 0.3048 | 역사적 문장 vs PRD 단일 출처 | README 표 + Domain golden만 `meters_per_*` (RR-1) |
| [ ] | **TD-4 표시 vs Domain golden 혼동** 8.2 vs 8.2021 | README 예시 1자리 | Boundary만 반올림 테스트 (tdd_rules) |
| [ ] | **TD-5 PRD FILE_NOT_FOUND 미결** (A/B) | Story AC “1종 고정” 미선택 | Should-Have 정책 확정 + README 동기화 |
| [ ] | **TD-6 Gherkin·PRD 권장 기능 갭** csv/json/register 미 BDD | Gherkin 8=table 위주 | v2 Scenario 추가 또는 IT로 대체 명시 |

---

## ✅ 완료 항목 (Done)

| ☑ | 완료 내용 | 완료일 | 관련 커밋/PR 메시지 |
|---|-----------|--------|---------------------|
| [x] | Phase 5 **PRD** (개요·기능·인수·회귀) 및 `docs/PRD.md` 인덱스 | 2026-05-20 | `docs: add Phase 5 PRD index` |
| [x] | Phase 6 **README** (계약·아키텍처·테스트·출력·기여) | 2026-05-20 | `docs: Phase 6 README from PRD` |
| [x] | **`.cursorrules`** (tdd, architecture, forbidden, testing) | 2026-05-20 | `docs: complete cursorrules for BCE TDD` |
| [x] | Phase 4 **Epic / Stories / Gherkin 8 / Level 5 체크리스트** (강의 산출물) | 2026-05-20 | `docs: requirements package traceability` |
| [x] | 스타터 **참고 구현** `UnitConverter.cpp` (학습 출발점) | (기존) | `chore: initial starter monolith` |

---

## 📋 회귀 방지 체크리스트 (PRD §7.2 / RR-1~6)

**배포·v1.0 태그·발표 전 — Alex 실행, 동료 서명**

| ☐ | 확인 항목 | 누가 | 통과 조건 |
|---|-----------|------|-----------|
| [ ] | **계약 테스트** | Alex | G-1~G-8 + regression_minimum **5/5 green** |
| [ ] | **stderr / exit 고정** | 동료 | RR-2: 매핑 테이블 변경 시 **연쇄 테스트** diff 포함 |
| [ ] | **golden / meters_per_*** | Alex | RR-1: 비율 변경 PR에 Domain + IT-OK-01 **동시** 갱신 |
| [ ] | **표시 1자리** | 동료 | RR-3: Domain ε 테스트 **불변**, Boundary만 수정 |
| [ ] | **register 문법** | Alex | RR-4: `register:` 변경 시 US-06 IT 갱신 |
| [ ] | **refactor green** | Alex | RR-5: 구조 PR 전 **전체 Catch2 green** 로그 첨부 |
| [ ] | **커버리지** | Alex | SC-1~2: entity ≥95% line, boundary ≥85% (리포트 첨부) |
| [ ] | **README 동기화** | 동료 | RR/README: exit·에러·비율 표·예시가 **실행 결과**와 일치 |
| [ ] | **forbidden 0건** | 동료 | cout 디버그·매직넘버·catch(...)·헤더구현·전역 registry |
| [ ] | **docs/PRD.md 인수 ☑** | 강사 | AC-1~7 게이트 표 전부 ☑ 또는 미통과 사유 |

---

## 🗓️ 마일스톤

| 마일스톤 | 포함 항목 (PRD) | 목표일 | 상태 | 완료 판정자 |
|----------|-----------------|--------|------|-------------|
| **M0 문서·계약** | PRD, README, .cursorrules, Gherkin, Epic | 2026-05-20 | **Done** | 강사 |
| **M1 Domain green** | F-03, F-01(핵심), US-03, SC-4, D-UC01 | T+3일 | Not started | Alex + 동료 |
| **M2 Boundary 필수** | F-02, F-04, F-05, F-06, US-01, G-3~8 | T+7일 | Not started | Alex |
| **M3 v1.0 인수** | Must-Have 전체, AC-1~4, RR-1~6, regression 5/5 | T+10일 | Not started | 강사 |
| **M4 품질 (Should)** | F-07~13, F-09~12, SC-1~2, US-04~06 | T+14일 | Not started | Alex |
| **M5 v2 후보** | Nice-to-Have, TD-5~6 해소 | TBD | Backlog | — |

*T+N일: M0 완료일(2026-05-20) 기준 상대 일정 — 팀 일정에 맞게 수정.*

---

## 추적 요약 (PRD 기능 → 마일스톤)

| PRD 기능 | Must | Should | Milestone |
|----------|------|--------|-----------|
| F-01 Convert | ✓ | | M1, M2 |
| F-02 검증 | ✓ | | M2 |
| F-03 Registry | ✓ | | M1 |
| F-04 table | ✓ | | M2 |
| F-05 unknown | ✓ | | M2 |
| F-06 Catch2 구조 | ✓ | | M1 |
| F-07 CSV | | ✓ | M4 |
| F-08 JSON | | ✓ | M4 |
| F-09 JSON config | | ✓ | M4 |
| F-10 register | | ✓ | M4 |
| F-11 config fail | | ✓ | M4 |
| F-12 duplicate | | ✓ | M4 |
| F-13 format reject | | ✓ | M4 |
| F-14 YAML | | | M5 |
| F-15 JSON stderr | | | M5 |

---

*코드 구현 없음. 항목 완료 시 `[ ]`→`[x]` 및 **Done** 섹션으로 이동.*
