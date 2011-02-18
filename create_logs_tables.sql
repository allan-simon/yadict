create table words_logs (
  "id"  integer PRIMARY KEY NOT NULL,       -- id of the event
  "user_id" integer  NOT NULL,              -- user who has made the event
  "event_type" integer  NOT NULL,           -- "1 : add/ 2 : delete/ 3 : edit" 
  "action_id" integer NOT NULL,             -- to know which atomic event is related to the same action
  "is_main" numeric  NOT NULL DEFAULT(1),  -- is this action the main one, or only a collateral effect ?
  "event_date" integer NOT NULL,            -- timestamp, when the event occured 

  "word_id" integer  NOT NULL,              -- word concerning by the event
  "lang" integer NOT NULL,                  -- lang of the word after the event     
  "text" text NOT NULL,                      -- "string" of the word after the event

  "prev_lang" integer DEFAULT NULL,      -- use for "edit" action to rollback
  "prev_text" text DEFAULT NULL           -- use for "edit" action to rollback
);

create index idx_words_logs_lang on words_logs(lang);
create index idx_words_logs_word_id on words_logs(word_id);
create index idx_words_logs_action_id on words_logs(action_id);

-----
-----

create table action_id ( last_id integer NOT NULL);
insert into action_id values (0);

-----
-----

create table metas_logs (
  "id"  integer PRIMARY KEY NOT NULL,       -- id of the event
  "user_id" integer  NOT NULL,              -- user who has made the event
  "event_type" integer  NOT NULL,           -- "1 : add/ 2 : delete/ 3 : edit" 
  "action_id" integer NOT NULL,             -- to know which atomic event is related to the same action
  "is_main" numeric  NOT NULL DEFAULT(1),  -- is this action the main one, or only a collateral effect ?
  "event_date" integer NOT NULL,            -- timestamp, when the event occured 

  "word_id" integer  NOT NULL,               -- word to which belongs the meta concerned by the event
  "word_lang" integer NOT NULL,
  "meta_key" integer NOT NULL,                    -- key of the meta after the event     
  "meta_value" text NOT NULL,                      -- value of the meta after the event

  "prev_key" integer DEFAULT NULL,        -- use for "edit" action to rollback
  "prev_value" text DEFAULT NULL           -- use for "edit" action to rollback
);
create index idx_metas_logs_lang on metas_logs(lang);
create index idx_metas_logs_word_id on metas_logs(word_id);
create index idx_metas_logs_action_id on metas_logs(action_id);

-----
-----

create table trans_logs (
  "id"  integer PRIMARY KEY NOT NULL,       -- id of the event
  "user_id" integer  NOT NULL,              -- user who has made the event
  "event_type" integer  NOT NULL,           -- "1 : add/ 2 : delete
  "action_id" integer NOT NULL,             -- to know which atomic event is related to the same action
  "is_main" numeric  NOT NULL DEFAULT(1),  -- is this action the main one, or only a collateral effect ?
  "event_date" integer NOT NULL,            -- timestamp, when the event occured 

  "word1_id" integer  NOT NULL,              -- first connected word
  "word2_id" integer NOT NULL               -- second one (order is not important)

);
create index idx_trans_logs_lang on trans_logs(lang);
create index idx_trans_logs_word1_id on trans_logs(word1_id);
create index idx_trans_logs_word2_id on trans_logs(word2_id);
create index idx_trans_logs_action_id on trans_logs(action_id);


