%w[
  rbconfig
  rake
  rake/clean
  rake/testtask
  rake/extensiontask
]
  .each(&method(:require))

SPEC = Gem::Specification.load(File.join(__dir__, 'xrb.gemspec'))

CLEAN.include(FileList['{ext,lib}/**/*.{so,o}'])
CLOBBER.include(FileList['tmp/**/*'])

%w[
  xrb/core
  xrb/window
  xrb/monitor
  xrb/pointer
  xrb/keyboard
  xrb/event
]
  .each do |task|
    Rake::ExtensionTask.new(task, SPEC) do |ext|
      ext.lib_dir = File.join('lib')
      ext.cross_compile = false
    end
  end

Rake::TestTask.new do |t|
  t.test_files = FileList['test/**/test*.rb']
end

task :console do
  chdir __dir__ do
    system 'bundle exec pry -r xrb'
  end
end

task build:    %i[clean compile]
task gauntlet: %i[build test console]
task default:  %i[gauntlet]
