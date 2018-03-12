Gem::Specification.new do |s|
  s.name        = 'xrb'
  s.version     = '0.0.3'

  s.platform    = Gem::Platform::RUBY

  s.license     = 'MIT'

  s.authors     = ['skotchpine']
  s.email       = ['tyler@skotchpine.com']

  s.homepage    = 'https://github.com/skotchpine/xrb'

  s.summary     = 'X11 for Ruby'
  s.description = 'The X11 Server in idiomatic Ruby'

  s.extensions =
    %w[
      core
      window
      monitor
      pointer
      keyboard
      event
    ]
      .map(&"ext/xrb/%s/extconf.rb".method(:%))

  s.files      = `git ls-files`.lines.map(&:chomp)
  s.test_files = s.files.select(&/test/.method(:=~))

  s.add_dependency('pry', '0.11.3')
end
