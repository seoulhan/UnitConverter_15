# UnitConverter — 결함 목록 (Defect List)

| 항목 | 내용 |
|------|------|
| **문서 버전** | 1.0 |
| **기준** | [test_plan.md](test_plan.md), [README.md](README.md), Catch2 `unit_converter_tests` (32 cases) |
| **검증 일자** | 2026-05-21 |
| **상태 요약** | BCE(`entity`/`boundary`/`data`) Catch2 **Green** · 레거시 `UnitConverter.cpp`·Windows 기본 CMake **Open** |

---

## 결함 등록표

| ID | Severity | 변환 타입 | 재현 절차 | 기대값 | 실제값 | 근본 원인 | 수정 요약 |
|----|----------|-----------|-----------|--------|--------|-----------|-----------|
| DEF-001 | Critical | (빌드) | `cmake -S . -B build && cmake --build build` (Windows, MSVC 미설치) | Configure·빌드 성공, `unit_converter_tests` 생성 | `nmake` not found, `CMAKE_CXX_COMPILER not set`, configure incomplete | Windows 기본 generator **NMake** 선택, 호스트에 MSVC Build Tools 없음 | `cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++` 후 빌드; README/`build.bat`에 명시 |
| DEF-002 | Critical | meter→feet | 레거시 실행: `unit_converter.exe`, stdin `meter:0` | 거부(exit **2**), stderr `Value must be positive: 0`, stdout 변환 줄 0 | `0 meter = 0 feet` 등 **0.000000** 출력, exit **1** | `UnitConverter.cpp`에 **value > 0** 검증 없음; `meterValue=0` 후 `* 3.28084` | `stod` 직후 `if (!(value > 0.0))` 검사 추가, README stderr·exit 2 정렬 |
| DEF-003 | Major | meter→feet | 레거시, stdin `meter:-1.0` | 거부(exit **2**), `Value must be positive: -1.0` | 음수 환산 출력(예: `-3.28084 feet`), exit **1** | 양수 정책 미구현 (DEF-002 동일 루트) | DEF-002와 동일 양수 검증 블록 |
| DEF-004 | Major | (CLI 계약) | 레거시, `meter2.5` / `meter:abc` / `furlong:1` | exit **2**(형식·숫자) 또는 **3**(unknown unit), 고정 stderr 패턴 | 모두 exit **1**, stderr 문구만 일부 유사 | `UnitConverter.cpp:17,27,40` `return 1` 하드코딩 | exit code를 README 표와 일치(2/3); 메시지는 README 문자열 그대로 |
| DEF-005 | Major | meter→feet (표시) | 레거시, stdin `meter:2.5` | table: `2.5 meter = 8.2 feet` (target **소수 1자리**), **source 단위 줄 없음**(D-UC01) | `2.5 meter = 8.2021 feet` 등 full precision + **`= … meter` target 줄 포함** | `UnitConverter.cpp:47-49` 3줄 고정 출력, Boundary Formatter 미사용 | source→meter 줄 제거; target만 `round(1)` (BCE `output_formatter` 위임 권장) |
| DEF-006 | Minor | meter→yard | 레거시 `meter:2.5` vs README table 예시 | `2.5 meter = 2.7 yard` (1자리) | `2.5 meter = 2.73403 yard` (raw double) | 표시 반올림 로직 없음 (DEF-005 동일) | Boundary 1자리 반올림 적용 또는 Formatter 연동 |
| DEF-007 | Minor | feet→meter (역변환) | 레거시 `feet:1` | `1 feet = 0.3 meter` (1자리 target) | `1 feet = 0.3048 meter` (full) | Formatter·반올림 없음 | DEF-005와 동일 |
| DEF-008 | Major | (아키텍처) | `UnitConverter.cpp` vs `entity/conversion_service.cpp` | 단일 환산식 `v_B=(v_A×R_A)/R_B`, 비율 JSON/constexpr 단일 출처 | 레거시에 literal `3.28084`/`1.09361` if-else (**OCP 위반**) | `UnitConverter.cpp:32-45` 단위별 분기·매직 넘버 | `main`→Control→Entity 위임; 레거시 monolith 제거(`.cursorrules` file_structure) |
| DEF-009 | Info | (문서/빌드) | README 「빠른 시작」만 따름 | Windows에서도 재현 가능한 빌드 명령 | MinGW/MSVC 구분 없음 → DEF-001 재발 | README에 generator/compiler 미기재 | README Quick Start에 MinGW 예시·`build.bat` 링크 추가 |
| DEF-010 | Info | (코드 품질) | 정적 검토 `UnitConverter.cpp:25` | Domain 경로: `DomainError`/명시적 실패 | `catch (...)` 포괄 삼킴 | `.cursorrules` forbidden `catch(...)` | `stod` 실패 시 명시 메시지·exit 2; catch 제거 또는 재throw |
| DEF-011 | Info | (RED 추적) | README RED Track A/B 체크리스트 | RED 완료 시 TC 체크 | Catch2 32건 **Green**이나 README RED 항목 **미체크** | 문서·실행 상태 미동기화 | RED 완료 항목 `[x]` 갱신; Open 결함은 본 문서 ID로 추적 |
| DEF-012 | Info | (커버리지) | `gcov`/`lcov` 미실행 | entity line ≥95%, boundary ≥85% | 커버리지 리포트 **미제출** | CI·로컬 lcov 파이프라인 미구축 | [test_plan.md](test_plan.md) §6 `build-cov` 절차 실행·리포트 첨부 |

---

## 심각도 정의 (본 프로젝트)

| Severity | 기준 |
|----------|------|
| **Critical** | 빌드 불가, 또는 정상 입력에서 변환 결과가 **완전히 무효**(0·미출력·치명 오류) |
| **Major** | 경계값·에러 계약(exit/stderr/양수) 위반, 역변환·unknown unit 오동작 |
| **Minor** | 계산은 맞으나 **출력 포맷·반올림·D-UC01** 불일치 |
| **Info** | 문서·스타일·추적·커버리지·아키텍처 부채 |

---

## 검증 메모

| 대상 | `meter:2.5` → feet golden | 비고 |
|------|---------------------------|------|
| `entity::ConversionService` | **8.2021** (ε 1e-5) | Catch2 `test_meter_to_feet_normal_returns_8_20210` **PASS** |
| 레거시 `UnitConverter.cpp` | **8.2021** (정상 경로) | feet 분기 **존재**; DEF-001 예시(0.000000)는 **`meter:0`** 등 비정상 입력에서 재현 |
| Windows `cmake -S . -B build` (기본) | — | DEF-001 **FAIL** (환경) |

---

## 수정 우선순위

1. **DEF-001** — 빌드 재현성 (MinGW/MSVC 명시)  
2. **DEF-002 ~ DEF-004** — 레거시 입력·exit 계약 (최소 if/return 수정)  
3. **DEF-005 ~ DEF-007** — 출력·D-UC01 (Formatter 연동 또는 줄 삭제+반올림)  
4. **DEF-008** — BCE 마이그레이션 (중장기)  
5. **DEF-009 ~ DEF-012** — 문서·커버리지·체크리스트 동기화  

---

## 회귀 확인 (결함 Close 시)

```powershell
cmake -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=g++
cmake --build build
.\build\unit_converter_tests.exe --reporter compact
ctest --test-dir build --output-on-failure
```

- **Close 조건:** 해당 ID 재현 절차에서 기대값 일치 + regression_minimum 5건 + 본 목록 ID **Fixed** 표기  
- **현재:** Catch2 전체 Green; DEF-001·002~008(레거시)은 **Open**

---

## 변경 이력

| 버전 | 날짜 | 변경 |
|------|------|------|
| 1.0 | 2026-05-21 | 최초 등록 (빌드·레거시 CLI·문서·커버리지) |
